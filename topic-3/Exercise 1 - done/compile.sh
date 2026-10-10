#!/bin/bash

# Compile homework.cpp into an executable named "homework".
# You may need to change the paths if your installation differs
C:\\raylib\\w64devkit\\bin\\g++ homework.cpp -IC:\\raylib\\w64devkit\\include -LC:\\raylib\\w64devkit\\lib -lraylib -lopengl32 -lgdi32 -lwinmm -g -o homework.exe
