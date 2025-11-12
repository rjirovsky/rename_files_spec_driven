@echo off
setlocal enabledelayedexpansion

REM Test with a file that already has RJ number in filename

set "test_file=test_folder\already_renamed_RJ-2023-99999.txt"

echo Testing with file: %test_file%
echo.

REM Parse filename
for %%F in ("%test_file%") do (
    set "file_name=%%~nF"
)

echo Filename: !file_name!
echo.

REM Check if file already contains RJ number pattern in filename
echo !file_name! | findstr /r "RJ-[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9][0-9][0-9]" >nul
if !errorlevel! equ 0 (
    echo Result: File already has RJ number in filename - SKIP
) else (
    echo Result: File does NOT have RJ number in filename - PROCEED
)

pause
