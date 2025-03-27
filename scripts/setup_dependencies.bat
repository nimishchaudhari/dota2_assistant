@echo off
REM This script sets up dependencies for the Dota 2 AI Assistant project

echo Setting up dependencies for Dota 2 AI Assistant...

REM Create directories if they don't exist
if not exist src\utils mkdir src\utils
if not exist external mkdir external

REM Download cpp-httplib
echo Downloading cpp-httplib from GitHub...
powershell -Command "Invoke-WebRequest -Uri 'https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h' -OutFile 'src\utils\httplib.h'"

if %ERRORLEVEL% neq 0 (
    echo Failed to download cpp-httplib
    exit /b 1
) else (
    echo Successfully downloaded cpp-httplib to src\utils\httplib.h
)

REM Download nlohmann_json
echo Downloading nlohmann_json...
powershell -Command "Invoke-WebRequest -Uri 'https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz' -OutFile 'external\json.tar.xz'"

if %ERRORLEVEL% neq 0 (
    echo Failed to download nlohmann_json
    echo Continuing anyway - will use online version if available
) else (
    echo Successfully downloaded nlohmann_json to external\json.tar.xz
)

REM Download GoogleTest
echo Downloading GoogleTest...
powershell -Command "Invoke-WebRequest -Uri 'https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip' -OutFile 'external\googletest-v1.13.0.zip'"

if %ERRORLEVEL% neq 0 (
    echo Failed to download GoogleTest
    echo Continuing anyway - will use online version if available
) else (
    echo Successfully downloaded GoogleTest to external\googletest-v1.13.0.zip
)

echo Dependencies setup complete!
