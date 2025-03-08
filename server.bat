@echo off

REM Run the Release executable in server mode
.\build\Release\atomic_engine.exe --server

if %errorlevel% neq 0 (
	echo Failed to run the application.
	exit /b %errorlevel%
)

echo Done!
