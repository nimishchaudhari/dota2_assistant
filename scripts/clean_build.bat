@echo off
REM This script completely cleans and rebuilds the project
echo Cleaning build directory...

REM Remove build directory if it exists
if exist build rmdir /s /q build

REM Run the setup script first
echo Running setup dependencies...
call scripts\setup_dependencies.bat

REM Generate build files
echo Generating build files...
cmake -B build -S .

REM Build the project
echo Building project...
cmake --build build --config Release

echo Build process completed!
echo You can now run tests with: cd build && ctest -C Release -R "GSIConnector|GameState"
