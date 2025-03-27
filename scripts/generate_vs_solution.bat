@echo off
REM This script generates a Visual Studio solution for the project

echo Generating Visual Studio solution...

REM Create the build directory if it doesn't exist
if not exist build mkdir build

REM Generate Visual Studio solution files
cmake -B build -S . -G "Visual Studio 17 2022" -A x64

echo Visual Studio solution generated in the 'build' directory.
echo You can open build/dota2_assistant.sln in Visual Studio.
