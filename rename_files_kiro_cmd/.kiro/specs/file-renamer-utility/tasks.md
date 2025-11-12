# Implementation Plan

- [x] 1. Create the main batch script structure
  - Create `rename_files.bat` in the project root
  - Set up script header with echo off and delayed expansion
  - Implement main entry point that calls subroutines
  - _Requirements: 1.1, 2.1_

- [x] 2. Implement initialization and variable setup
  - Initialize counters for tracking processed and renamed files
  - Set up environment variables for script operation
  - Display startup message to user
  - _Requirements: 3.5_

- [x] 3. Implement file discovery functionality
  - Write code to recursively find all .txt files using `dir /s /b *.txt`
  - Store discovered file paths for processing
  - Count total files found
  - _Requirements: 1.1, 2.1, 2.2, 2.4_

- [x] 4. Implement pattern extraction logic
  - Create subroutine to search file content using PowerShell regex
  - Implement regex pattern matching for RJ-YYYY-NNNNN format
  - Extract first occurrence of RJ number
  - Handle files with no pattern found
  - _Requirements: 1.2, 1.5_

- [x] 5. Implement filename generation and validation
  - Parse original filename and extension from full path
  - Generate new filename in format `<original_name>_<rj_number>.<extension>`
  - Check if file already contains RJ number pattern in filename
  - Validate new filename doesn't already exist
  - _Requirements: 1.3, 1.4, 3.3_

- [x] 6. Implement file renaming operation
  - Execute rename command using `ren` with proper path handling
  - Implement error handling for file access issues
  - Update renamed file counter on success
  - Continue processing on errors without terminating script
  - _Requirements: 3.2, 3.4_

- [x] 7. Implement results reporting
  - Display summary of total files processed
  - Show count of successfully renamed files
  - Provide clear completion message
  - _Requirements: 3.5_

- [x] 8. Create test folder structure and sample files
  - Create `testing/` directory with subdirectories
  - Create `testing/basic_tests/` with simple test files
  - Create `testing/nested_tests/level1/level2/` with nested files
  - Create `testing/edge_cases/` with edge case test files
  - Generate sample .txt files with various RJ patterns
  - Generate files without patterns for negative testing
  - _Requirements: 1.1, 2.1, 2.3_
