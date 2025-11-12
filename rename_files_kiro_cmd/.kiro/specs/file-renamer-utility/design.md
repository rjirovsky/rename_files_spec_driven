# Design Document

## Overview

The File Renamer Utility is a Windows batch script that automatically renames .txt files by extracting RJ numbers from their content. The system uses pattern matching to find RJ-YYYY-NNNNN formatted identifiers and appends them to filenames for improved organization.

## Architecture

### High-Level Flow
1. **Initialization**: Set up environment variables and enable delayed expansion
2. **Discovery**: Recursively find all .txt files in working directory and subdirectories
3. **Processing**: For each file, search content for RJ numbers patterns
4. **Extraction**: Parse the first valid RJ-YYYY-NNNNN pattern found
5. **Validation**: Verify pattern format and check if file already renamed
6. **Renaming**: Apply new filename format preserving original name and extension
7. **Reporting**: Provide summary of processed and renamed files

### Core Components

#### 1. File Discovery Engine
- Uses `dir /s /b *.txt` to recursively find all .txt files
- Maintains full path information for proper renaming
- Handles nested directory structures

#### 2. Pattern Matching Engine
- Uses `findstr` with regex pattern to locate RJ numbers
- Pattern: `RJ-[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9][0-9][0-9]`
- Extracts first occurrence if multiple patterns exist

#### 3. Filename Generator
- Parses original filename and extension
- Constructs new name: `<original_name>_<rj_number>.<extension>`
- Validates that new filename doesn't already exist

#### 4. File Operations Manager
- Performs safe file renaming using `ren` command
- Implements error handling for file access issues
- Skips files that are already renamed or inaccessible

## Components and Interfaces

### Main Script Structure

```batch
@echo off
setlocal enabledelayedexpansion

:main
    call :initialize
    call :discover_files
    call :process_files
    call :report_results
    goto :eof

:initialize
    - Set counters and variables
    - Display startup message

:discover_files
    - Find all .txt files recursively
    - Store file list for processing

:process_files
    - Loop through each file
    - Extract RJ number
    - Rename if pattern found

:extract_pattern
    - Search file content for RJ-YYYY-NNNNN
    - Return first valid pattern found

:rename_file
    - Generate new filename
    - Perform rename operation
    - Handle errors gracefully

:report_results
    - Display processing summary
    - Show success/failure counts
```

### Key Variables

- `file_count`: Total .txt files found
- `renamed_count`: Successfully renamed files
- `current_file`: File being processed
- `rj_number`: Extracted RJ pattern
- `new_filename`: Generated filename

## Data Models

### File Information Structure
- **Full Path**: Complete path to file including directory
- **Filename**: Base name without extension
- **Extension**: File extension (.txt)
- **Content**: File content for pattern searching
- **RJ number**: Extracted RJ-YYYY-NNNNN pattern

### Pattern Structure
- **Prefix**: "RJ-" (fixed)
- **Year**: Four digits (YYYY)
- **Separator**: "-" (fixed)
- **Number**: Five digits (NNNNN)
- **Full Pattern**: RJ-YYYY-NNNNN

## Error Handling

### File Access Errors
- **Issue**: File locked or permission denied
- **Handling**: Skip file, log error, continue processing
- **User Feedback**: Display warning message

### Pattern Extraction Errors
- **Issue**: No valid pattern found in file
- **Handling**: Skip file, no error logged
- **User Feedback**: File not processed (normal behavior)

### Filename Conflicts
- **Issue**: Target filename already exists
- **Handling**: Skip rename to prevent overwrite
- **User Feedback**: Display conflict warning

### Invalid Characters
- **Issue**: Special characters in extracted pattern
- **Handling**: Sanitize filename or skip if unsafe
- **User Feedback**: Display sanitization warning

## Testing Strategy

### Unit Testing Approach
Since batch scripts don't have traditional unit testing frameworks, testing will be manual and scenario-based:

### Test Scenarios

#### 1. Basic Functionality Tests
- Single file with valid RJ pattern
- Multiple files in same directory
- Files in nested subdirectories
- Files without RJ patterns (should be skipped)

#### 2. Pattern Validation Tests
- Various year formats (RJ-2023-12345, RJ-2024-67890)
- Different number sequences
- Multiple patterns in same file (use first)
- Malformed patterns (should be ignored)

#### 3. Edge Case Tests
- Files already renamed (should be skipped)
- Empty files
- Very large files
- Files with special characters in names
- Read-only files

#### 4. Error Handling Tests
- Locked files
- Permission denied scenarios
- Invalid file paths
- Disk space issues

### Test Data Structure
```
testing/
├── basic_tests/
│   ├── simple_file.txt (contains RJ-2024-12345)
│   ├── no_pattern.txt (no RJ pattern)
│   └── multiple_patterns.txt (contains RJ-2024-11111 and RJ-2024-22222)
├── nested_tests/
│   ├── level1/
│   │   ├── nested_file.txt (contains RJ-2023-99999)
│   │   └── level2/
│   │       └── deep_file.txt (contains RJ-2025-55555)
├── edge_cases/
│   ├── already_renamed_RJ-2024-12345.txt
│   ├── empty_file.txt
│   └── special_chars_file.txt
└── error_tests/
    └── readonly_file.txt (set as read-only)
```

### Validation Criteria
- All files with valid patterns are renamed correctly
- Files without patterns remain unchanged
- Directory structure is preserved
- No data loss occurs during renaming
- Error messages are clear and helpful
- Processing summary is accurate