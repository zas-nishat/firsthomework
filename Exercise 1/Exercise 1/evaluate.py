import subprocess
import zipfile
from pathlib import Path


# ============================================================
# CONFIGURATION
# ============================================================

PROGRAM_ARGS = [
    ["Hello, World!"],
    ["Why is 2+2=4? Strange."],
]

EXPECTED_OUTPUTS = [
    "eoo HllWrld",
    "yiae WhsStrng",
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
        if outputs[i] != EXPECTED_OUTPUTS[i]:
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


# def run_homework(student_dir: Path):
#     """
#     Run homework.exe once for every argument list.

#     Returns:
#         (success, outputs, error)

#     `outputs` is a list containing one string per invocation.
#     """

#     exe = student_dir / "homework.exe"

#     if not exe.is_file():
#         return False, [], "missing homework.exe"

#     outputs = []

#     for args in PROGRAM_ARGS:

#         success, output = run_command(
#             [str(exe)] + args,
#             cwd=student_dir,
#         )

#         if not success:
#             return False, outputs, f"program failed with arguments: {args}: {output or 'no error output'}"

#         outputs.append(output)

#     return True, outputs, None

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


# ============================================================
# MAIN
# ============================================================

def main():

    base_dir = Path(__file__).resolve().parent

    zip_files = sorted(base_dir.glob("*.zip"))

    if not zip_files:
        print("No .zip files found.")
        return

    results = []

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
        # 2. Compile
        # ----------------------------------------------------

        success, error = compile_student(student_dir)

        if not success:
            results.append((student_name, error))
            continue

        # ----------------------------------------------------
        # 3. Run program multiple times
        # ----------------------------------------------------

        success, outputs, error = run_homework(student_dir)

        if not success:
            results.append((student_name, error))
            continue

        # ----------------------------------------------------
        # 4. Evaluate all outputs
        # ----------------------------------------------------

        try:
            evaluation = evaluate_output(outputs)
            results.append((student_name, evaluation))

        except Exception as e:
            results.append((student_name, f"evaluation failed: {e}"))

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