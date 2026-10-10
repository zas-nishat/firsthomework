:: Don't echo each command to the console
@echo off
:: Compile homework.cpp into homework.exe.
:: you may need to change the paths if your installation differs
C:\\raylib\\w64devkit\\bin\\g++.exe homework.cpp -IC:\\raylib\\w64devkit\\include -LC:\\raylib\\w64devkit\\lib -lraylib -lopengl32 -lgdi32 -lwinmm -o homework.exe

:: ERRORLEVEL contains the exit code of the last command.
:: g++ returns 0 if compilation succeeded and a non-zero value if it failed.
if %ERRORLEVEL% NEQ 0 (
    :: Tell Python that compilation failed.
    exit /b 1
)

:: Tell Python that compilation succeeded.
exit /b 0
