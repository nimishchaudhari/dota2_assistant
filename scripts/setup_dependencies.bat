@echo off
REM This script sets up dependencies for the Dota 2 AI Assistant project

echo Downloading cpp-httplib...
if not exist src\utils mkdir src\utils

REM Download cpp-httplib
echo Downloading cpp-httplib from GitHub...
powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h' -OutFile 'src\utils\httplib.h'"

if %ERRORLEVEL% neq 0 (
    echo Failed to download cpp-httplib
    exit /b 1
)

echo Dependencies setup complete!
