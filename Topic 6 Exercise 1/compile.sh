#!/bin/bash

# Compile homework.cpp into homework (no .exe on Mac/Linux).
# -g includes debugging information (useful if you run it in a debugger).
g++ homework.cpp -g -o homework

# $? is the exit code of the last command.
# g++ returns 0 if compilation succeeded and a non-zero value if it failed.
# Tell Python whether compilation succeeded.
if [ $? -ne 0 ]; then
    exit 1
fi

exit 0