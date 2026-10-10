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

IS_WINDOWS = sys.platform.startswith("win")

# Platform-specific build script and executable names.
COMPILE_SCRIPT = "compile.bat" if IS_WINDOWS else "compile.sh"
COMPILE_CMD    = ["cmd.exe", "/c", "compile.bat"] if IS_WINDOWS else ["bash", "compile.sh"]
EXE_NAME       = "homework.exe" if IS_WINDOWS else "homework"

# Seconds before a run is considered hung (e.g. infinite loop).
RUN_TIMEOUT = 10

# Common Windows crash codes (NTSTATUS), shown instead of a debugger report.
WINDOWS_CRASH_CODES = {
    0xC0000005: "access violation (invalid memory access, e.g. bad pointer / out-of-bounds)",
    0xC00000FD: "stack overflow (e.g. infinite recursion)",
    0xC0000094: "integer division by zero",
    0xC0000374: "heap corruption",
    0xC0000409: "stack buffer overrun / fast-fail",
    0xC000001D: "illegal instruction",
    0xC0000096: "privileged instruction",
}

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
    """Run the compile script and verify the executable was created."""

    compile_script = student_dir / COMPILE_SCRIPT

    if not compile_script.is_file():
        return False, f"missing {COMPILE_SCRIPT}"

    success, output = run_command(COMPILE_CMD, cwd=student_dir)

    if not success:
        return False, f"compilation failed: {output}"

    exe = student_dir / EXE_NAME

    if not exe.is_file():
        return False, f"{EXE_NAME} not produced"

    return True, None


def describe_crash(returncode: int) -> str:
    """Turn a process exit code into a human-readable explanation."""

    if IS_WINDOWS:
        code = returncode & 0xFFFFFFFF
        known = WINDOWS_CRASH_CODES.get(code)
        if known:
            return f"crashed with 0x{code:08X}: {known}"
        return f"exited with non-zero code {returncode} (0x{code:08X})"

    # POSIX: negative return code means "killed by signal N"
    if returncode < 0:
        try:
            name = signal.Signals(-returncode).name
        except ValueError:
            name = f"signal {-returncode}"
        hints = {
            "SIGSEGV": "segmentation fault (invalid memory access)",
            "SIGBUS": "bus error (invalid memory access)",
            "SIGFPE": "arithmetic error (e.g. division by zero)",
            "SIGABRT": "aborted (e.g. failed assert, double free, heap corruption)",
            "SIGILL": "illegal instruction",
        }
        return f"crashed with {name}" + (f": {hints[name]}" if name in hints else "")

    return f"exited with non-zero code {returncode}"


def run_homework(student_dir: Path):
    exe = student_dir / EXE_NAME

    if not exe.is_file():
        return False, [], f"missing {EXE_NAME}"

    outputs = []

    for args in PROGRAM_ARGS:
        try:
            result = subprocess.run(
                [str(exe), *args],
                cwd=student_dir,
                capture_output=True,
                text=True,
                timeout=RUN_TIMEOUT,
            )
        except subprocess.TimeoutExpired:
            return False, outputs, (
                f"program timed out after {RUN_TIMEOUT}s with arguments: {args} "
                f"(infinite loop?)"
            )

        if result.returncode == 0:
            outputs.append(result.stdout.strip())
            continue

        # Program failed/crashed: report what we know, no debugger needed.
        details = describe_crash(result.returncode)
        stderr = result.stderr.strip()
        stdout = result.stdout.strip()

        message = f"program failed with arguments: {args}\n{details}"
        if stderr:
            message += f"\nstderr:\n{stderr}"
        if stdout:
            message += f"\nstdout before failure:\n{stdout}"

        return False, outputs, message

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

        student_name = zip_path.stem.strip(" .")
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