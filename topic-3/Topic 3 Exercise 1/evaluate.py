import re
import subprocess
import sys
import zipfile
from pathlib import Path


# ============================================================
# CONFIGURATION
# ============================================================

PROGRAM_ARGS = [
    ["2", "2", "6", "7"],
    ["0", "3", "7", "7"],
    ["4", "5", "5", "4"],
    ["0", "7", "7", "7"],
    ["2", "1", "7", "2"],
]

EXPECTED_OUTPUTS = [
    "3",
    "5",
    "2",
    "5",
    "4",
]

GREEN = "\033[92m"
RED   = "\033[91m"
RESET = "\033[0m"

SOURCE_NAME = "homework.cpp"
SOLUTION_NAME = "solution.cpp"
TEMP_BAT_NAME = "_compile_solution.bat"


def evaluate_output(outputs: list[str]) -> str:
    """
    Process all outputs produced by homework.exe.

    `outputs[0]` corresponds to PROGRAM_ARGS[0]
    `outputs[1]` corresponds to PROGRAM_ARGS[1]
    etc.

    Replace this with your own evaluation logic.
    """

    # Simple accuracy score + explanation
    n_tests = len(PROGRAM_ARGS)
    n_fails = 0
    str_fails = ""
    for i in range(n_tests):
        if outputs[i].strip() != EXPECTED_OUTPUTS[i].strip():
            n_fails += 1
            str_fails += f"{RED}\nargs {PROGRAM_ARGS[i]} should provide {EXPECTED_OUTPUTS[i]} but got {outputs[i]}{RESET}"

    return f"{n_tests - n_fails}/{n_tests}." + str_fails

# ============================================================
# HELPERS
# ============================================================

def run_command(command, cwd):
    """Run a command and return (success, output)."""

    try:
        result = subprocess.run(
            command,
            cwd=cwd,
            capture_output=True,
            text=True,
            shell=False,
        )

        output = (result.stdout + "\n" + result.stderr).strip()

        return result.returncode == 0, output

    except Exception as e:
        return False, str(e)


def compile_student(student_dir: Path, use_solution: bool = False, script_dir: Path = None):
    """Run compile.bat and verify an exe was created.

    If use_solution is True, compile.bat is NOT modified. Instead, a temporary
    copy with homework.cpp replaced by solution.cpp is written, run, and deleted.

    Returns (success, error, exe_path).
    """

    compile_bat = student_dir / "compile.bat"
    exe = student_dir / "homework.exe"

    if not compile_bat.is_file():
        return False, "missing compile.bat", None

    temp_bat = None

    if use_solution:
        solution_cpp = student_dir / SOLUTION_NAME

        # In zip mode, solution.cpp likely lives next to this script
        if not solution_cpp.is_file() and script_dir is not None:
            shared = script_dir / SOLUTION_NAME
            if shared.is_file():
                solution_cpp.write_bytes(shared.read_bytes())

        if not solution_cpp.is_file():
            return False, f"missing {SOLUTION_NAME}", None

        original = compile_bat.read_bytes()
        patched, n_subs = re.subn(
            re.escape(SOURCE_NAME.encode()),
            SOLUTION_NAME.encode(),
            original,
            flags=re.IGNORECASE,
        )

        if n_subs == 0:
            return False, f"compile.bat does not mention {SOURCE_NAME}, cannot substitute", None

        temp_bat = student_dir / TEMP_BAT_NAME
        temp_bat.write_bytes(patched)
        bat_to_run = TEMP_BAT_NAME
    else:
        bat_to_run = "compile.bat"

    # Remove stale executables so we don't accidentally run an old build
    for stale in (exe, student_dir / "solution.exe"):
        if stale.is_file():
            try:
                stale.unlink()
            except OSError:
                pass

    try:
        success, output = run_command(
            ["cmd.exe", "/c", bat_to_run],
            cwd=student_dir,
        )
    finally:
        if temp_bat is not None and temp_bat.is_file():
            temp_bat.unlink()

    if not success:
        return False, f"compilation failed: {output}", None

    if exe.is_file():
        return True, None, exe

    # If compile.bat doesn't name the output explicitly, the compiler
    # may have named it after the source file (solution.exe).
    if use_solution:
        alt = student_dir / "solution.exe"
        if alt.is_file():
            return True, None, alt

    return False, "homework.exe not produced", None


def run_homework(student_dir: Path, exe: Path):
    if not exe.is_file():
        return False, [], f"missing {exe.name}"

    outputs = []

    for args in PROGRAM_ARGS:
        result = subprocess.run(
            [str(exe), *args],
            cwd=student_dir,
            capture_output=True,
            text=True,
        )

        if result.returncode == 0:
            outputs.append(result.stdout.strip())
            continue

        # Program failed/crashed. Run it through CDB to diagnose it.
        cdb_result = subprocess.run(
            [
                "cdb",
                "-c",
                "g; !analyze -v -forceuser; q",
                "-o",
                str(exe),
                *args,
            ],
            cwd=student_dir,
            capture_output=True,
            text=True,
        )

        error = cdb_result.stdout.strip()

        return False, outputs, (
            f"program failed with arguments: {args}\n{error}"
        )

    return True, outputs, None


def grade_one(student_name: str, student_dir: Path, use_solution: bool = False, script_dir: Path = None):
    """Compile, run, and evaluate a single student directory.

    Returns a (student_name, result_str) tuple, matching the shape
    used in the results list.
    """

    # ----------------------------------------------------
    # Compile
    # ----------------------------------------------------

    success, error, exe = compile_student(student_dir, use_solution, script_dir)

    if not success:
        return (student_name, error)

    # ----------------------------------------------------
    # Run program multiple times
    # ----------------------------------------------------

    success, outputs, error = run_homework(student_dir, exe)

    if not success:
        return (student_name, error)

    # ----------------------------------------------------
    # Evaluate all outputs
    # ----------------------------------------------------

    try:
        evaluation = evaluate_output(outputs)
        return (student_name, evaluation)

    except Exception as e:
        return (student_name, f"evaluation failed: {e}")


# ============================================================
# MAIN
# ============================================================

def main():

    base_dir = Path(__file__).resolve().parent

    args = sys.argv[1:]
    run_here = "-here" in args
    use_solution = "-solution" in args

    results = []

    # --------------------------------------------------------
    # "-here" mode: skip zip handling, grade the current folder
    # directly (compile.bat + homework.exe already in place).
    # --------------------------------------------------------

    if run_here:

        print(f"Processing (in place): {base_dir.name}"
              + (" [using solution.cpp]" if use_solution else ""))

        results.append(grade_one(base_dir.name, base_dir, use_solution, base_dir))

        print()
        print("=" * 60)
        print("RESULTS")
        print("=" * 60)

        for student_name, result in results:
            print(f"{GREEN}{student_name}{RESET}: {result}")

        return

    # --------------------------------------------------------
    # Normal mode: unzip each submission and grade it
    # --------------------------------------------------------

    zip_files = sorted(base_dir.glob("*.zip"))

    if not zip_files:
        print("No .zip files found.")
        return

    for zip_path in zip_files:

        student_name = zip_path.stem.strip(" .")
        student_dir = base_dir / student_name

        print(f"Processing: {student_name}"
              + (" [using solution.cpp]" if use_solution else ""))

        # ----------------------------------------------------
        # 1. Extract ZIP
        # ----------------------------------------------------

        try:
            student_dir.mkdir(exist_ok=True)

            with zipfile.ZipFile(zip_path, "r") as zip_file:
                zip_file.extractall(student_dir)

        except zipfile.BadZipFile:
            results.append((student_name, "invalid zip"))
            continue

        except Exception as e:
            results.append((student_name, f"extraction failed: {e}"))
            continue

        # ----------------------------------------------------
        # 2-4. Compile, run, evaluate
        # ----------------------------------------------------

        results.append(grade_one(student_name, student_dir, use_solution, base_dir))

    # ========================================================
    # FINAL RESULTS
    # ========================================================

    print()
    print("=" * 60)
    print("RESULTS")
    print("=" * 60)

    for student_name, result in results:
        print(f"{GREEN}{student_name}{RESET}: {result}")


if __name__ == "__main__":
    main()