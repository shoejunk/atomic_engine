@echo off

REM Get the server IP address from the command line or use localhost as default
set SERVER_IP=127.0.0.1
if not "%~1"=="" set SERVER_IP=%~1

echo Connecting to server at %SERVER_IP%...

REM Run the Release executable in client mode
.\build\Release\atomic_engine.exe --client %SERVER_IP%

set EXIT_CODE=%errorlevel%
if %EXIT_CODE% neq 0 (
	echo Failed to run the application. Error code: %EXIT_CODE%
	pause
	exit /b %EXIT_CODE%
)

echo Done!
