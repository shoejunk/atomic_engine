@echo off

REM Run the Debug executable in server mode
.\build\Debug\atomic_engine.exe --server

if %errorlevel% neq 0 (
	echo Failed to run the application.
	exit /b %errorlevel%
)

echo Done!
