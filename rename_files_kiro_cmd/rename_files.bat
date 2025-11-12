@echo off
setlocal enabledelayedexpansion

REM ============================================================================
REM File Renamer Utility
REM Automatically renames .txt files based on RJ numbers found in content
REM Pattern: RJ-YYYY-NNNNN
REM ============================================================================

:main
    call :initialize
    call :discover_files
    call :process_files
    call :report_results
    goto :eof

REM ----------------------------------------------------------------------------
REM Initialize counters and environment variables
REM ----------------------------------------------------------------------------
:initialize
    set file_count=0
    set renamed_count=0
    set processed_count=0
    
    echo.
    echo ============================================================================
    echo File Renamer Utility
    echo ============================================================================
    echo.
    echo Searching for .txt files with RJ numbers...
    echo Pattern: RJ-YYYY-NNNNN
    echo.
    
    goto :eof

REM ----------------------------------------------------------------------------
REM Discover all .txt files recursively
REM ----------------------------------------------------------------------------
:discover_files
    echo Discovering .txt files...
    echo.
    
    REM Use dir command to recursively find all .txt files
    REM /s = search subdirectories
    REM /b = bare format (full path only)
    for /f "delims=" %%f in ('dir /s /b *.txt 2^>nul') do (
        set /a file_count+=1
    )
    
    echo Found !file_count! .txt file(s) to process
    echo.
    
    goto :eof

REM ----------------------------------------------------------------------------
REM Process each discovered file
REM ----------------------------------------------------------------------------
:process_files
    echo Processing files...
    echo.
    
    REM Loop through all .txt files recursively
    for /f "delims=" %%f in ('dir /s /b *.txt 2^>nul') do (
        set "current_file=%%f"
        set /a processed_count+=1
        
        REM Extract RJ number pattern from file content
        call :extract_pattern "!current_file!"
        
        REM If pattern found, attempt to rename the file
        if defined rj_number (
            call :rename_file "!current_file!" "!rj_number!"
        )
    )
    
    echo.
    goto :eof

REM ----------------------------------------------------------------------------
REM Extract RJ number pattern from file content
REM Parameters: %1 = file path
REM Returns: rj_number variable (empty if no pattern found)
REM ----------------------------------------------------------------------------
:extract_pattern
    set "rj_number="
    set "file_path=%~1"
    
    REM Use PowerShell to extract the first RJ pattern from the file
    REM This is more reliable than pure batch string manipulation
    for /f "usebackq delims=" %%p in (`powershell -NoProfile -Command "$content = Get-Content '%file_path%' -Raw -ErrorAction SilentlyContinue; if ($content -match 'RJ-\d{4}-\d{5}') { $matches[0] }"`) do (
        set "rj_number=%%p"
        goto :extract_pattern_done
    )
    
:extract_pattern_done
    goto :eof

REM ----------------------------------------------------------------------------
REM Rename file with RJ number
REM Parameters: %1 = file path, %2 = RJ number
REM ----------------------------------------------------------------------------
:rename_file
    set "file_path=%~1"
    set "rj_number=%~2"
    
    REM Parse original filename and extension from full path
    set "file_dir=%~dp1"
    set "file_name=%~n1"
    set "file_ext=%~x1"
    
    REM Check if file already contains RJ number pattern in filename
    echo !file_name! | findstr /r "RJ-[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9][0-9][0-9]" >nul
    if !errorlevel! equ 0 (
        REM File already has RJ number in filename, skip it
        goto :eof
    )
    
    REM Generate new filename in format <original_name>_<rj_number>.<extension>
    set "new_filename=!file_name!_!rj_number!!file_ext!"
    set "new_filepath=!file_dir!!new_filename!"
    
    REM Validate new filename doesn't already exist
    if exist "!new_filepath!" (
        REM Target filename already exists, skip to avoid overwrite
        echo WARNING: Target file already exists: !new_filename!
        goto :eof
    )
    
    REM Execute rename command with proper path handling
    REM Change to the file's directory to handle paths correctly
    pushd "!file_dir!"
    
    REM Perform the rename operation
    ren "!file_name!!file_ext!" "!new_filename!" 2>nul
    
    REM Check if rename was successful
    if !errorlevel! equ 0 (
        REM Rename successful - update counter and display message
        set /a renamed_count+=1
        echo Renamed: !file_name!!file_ext! -^> !new_filename!
    ) else (
        REM Rename failed - display error but continue processing
        echo ERROR: Failed to rename !file_name!!file_ext! ^(file may be locked or access denied^)
    )
    
    REM Return to original directory
    popd
    
    goto :eof

REM ----------------------------------------------------------------------------
REM Display processing results summary
REM ----------------------------------------------------------------------------
:report_results
    echo ============================================================================
    echo Processing Complete
    echo ============================================================================
    echo.
    echo Total files processed: !processed_count!
    echo Successfully renamed:  !renamed_count!
    echo.
    
    REM Provide contextual completion message based on results
    if !renamed_count! equ 0 (
        if !processed_count! equ 0 (
            echo No .txt files found in the current directory or subdirectories.
        ) else (
            echo No files were renamed. Files may already be renamed or contain no RJ number patterns.
        )
    ) else (
        echo File renaming completed successfully.
    )
    
    echo.
    echo ============================================================================
    echo.
    
    goto :eof
