@echo off

REM Build the project and redirect output to build_output.txt
cmake --build build --config Release > build_output.txt 2>&1

REM Check if the build was successful
if %ERRORLEVEL% EQU 0 (
	echo Build succeeded! Output saved to build_output.txt
) else (
	echo Build failed! Check build_output.txt for details
	exit /b 1
)
