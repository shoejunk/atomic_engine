@echo off

REM Run the Release executable
.\build\Release\atomic_engine.exe

if %errorlevel% neq 0 (
    echo Failed to run the application.
    exit /b %errorlevel%
)

echo Done!
