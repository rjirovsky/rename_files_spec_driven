@echo off
setlocal enabledelayedexpansion

REM Test the filename generation and validation logic

set "test_file=test_folder\test_pattern.txt"
set "rj_number=RJ-2024-12345"

REM Parse original filename and extension from full path
set "file_dir=%~dp1"
set "file_name=%~n1"
set "file_ext=%~x1"

echo Testing with file: %test_file%
echo RJ number: %rj_number%
echo.

REM Use for loop to parse the filename components
for %%F in ("%test_file%") do (
    set "file_dir=%%~dpF"
    set "file_name=%%~nF"
    set "file_ext=%%~xF"
)

echo Parsed components:
echo   Directory: !file_dir!
echo   Filename: !file_name!
echo   Extension: !file_ext!
echo.

REM Check if file already contains RJ number pattern in filename
echo !file_name! | findstr /r "RJ-[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9][0-9][0-9]" >nul
if !errorlevel! equ 0 (
    echo File already has RJ number in filename - SKIP
) else (
    echo File does NOT have RJ number in filename - PROCEED
)
echo.

REM Generate new filename
set "new_filename=!file_name!_!rj_number!!file_ext!"
set "new_filepath=!file_dir!!new_filename!"

echo Generated new filename:
echo   New filename: !new_filename!
echo   New filepath: !new_filepath!
echo.

REM Check if new file exists
if exist "!new_filepath!" (
    echo WARNING: Target file already exists
) else (
    echo Target file does not exist - OK to rename
)

pause
