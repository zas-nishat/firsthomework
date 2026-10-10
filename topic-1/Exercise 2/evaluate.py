import subprocess
import sys
import zipfile
from pathlib import Path


# ============================================================
# CONFIGURATION
# ============================================================

PROGRAM_ARGS = [
    ["0", "0", "3", "4"],
    ["-5", "12", "7", "14"],
    ["10", "0", "20", "0"],
]

EXPECTED_OUTPUTS = [
    "5.00",
    "12.17",
    "10.00",
]

GREEN = "\033[92m"
RED   = "\033[91m"
RESET = "\033[0m"

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


def compile_student(student_dir: Path):
    """Run compile.bat and verify homework.exe was created."""

    compile_bat = student_dir / "compile.bat"

    if not compile_bat.is_file():
        return False, "missing compile.bat"

    success, output = run_command(
        ["cmd.exe", "/c", "compile.bat"],
        cwd=student_dir,
    )

    if not success:
        return False, f"compilation failed: {output}"

    exe = student_dir / "homework.exe"

    if not exe.is_file():
        return False, "homework.exe not produced"

    return True, None

def run_homework(student_dir: Path):
    exe = student_dir / "homework.exe"

    if not exe.is_file():
        return False, [], "missing homework.exe"

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


def grade_one(student_name: str, student_dir: Path):
    """Compile, run, and evaluate a single student directory.

    Returns a (student_name, result_str) tuple, matching the shape
    used in the results list.
    """

    # ----------------------------------------------------
    # Compile
    # ----------------------------------------------------

    success, error = compile_student(student_dir)

    if not success:
        return (student_name, error)

    # ----------------------------------------------------
    # Run program multiple times
    # ----------------------------------------------------

    success, outputs, error = run_homework(student_dir)

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

    run_here = "-here" in sys.argv[1:]

    results = []

    # --------------------------------------------------------
    # "-here" mode: skip zip handling, grade the current folder
    # directly (compile.bat + homework.exe already in place).
    # --------------------------------------------------------

    if run_here:

        print(f"Processing (in place): {base_dir.name}")

        results.append(grade_one(base_dir.name, base_dir))

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

        student_name = zip_path.stem
        student_dir = base_dir / student_name

        print(f"Processing: {student_name}")

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

        results.append(grade_one(student_name, student_dir))

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