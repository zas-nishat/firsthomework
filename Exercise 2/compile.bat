:: Don't echo each command to the console
@echo off

:: Compile homework.cpp into homework.exe.
:: -g includes debugging information, which helps CDB identify
:: source files and line numbers when the program crashes.
g++ homework.cpp -g -o homework.exe

:: ERRORLEVEL contains the exit code of the last command.
:: g++ returns 0 if compilation succeeded and a non-zero value if it failed.
if %ERRORLEVEL% NEQ 0 (
    :: Tell Python that compilation failed.
    exit /b 1
)

:: Tell Python that compilation succeeded.
exit /b 0
