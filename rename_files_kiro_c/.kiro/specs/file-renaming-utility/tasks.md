# Implementation Plan

- [x] 1. Set up project structure and core definitions





  - Create main C source file (rename_files.c)
  - Define constants (MAX_PATH_LENGTH, RJ_PATTERN_LENGTH)
  - Define Statistics structure for tracking file operations
  - Add necessary includes (stdio.h, stdlib.h, string.h, windows.h)
  - _Requirements: 1.1, 2.1, 3.1, 5.4_
-

- [x] 2. Implement pattern matching module








  - _Requirements: 1.1, 1.2, 1.3, 1.4, 1.5_

- [x] 2.1 Write RJ pattern extraction function


  - Implement `extract_rj_pattern()` to search for "RJ-" prefix in file content
  - Extract complete RJ-YYYY-NNNNN pattern from text
  - Return first valid occurrence when multiple patterns exist
  - Handle case when no pattern is found
  - _Requirements: 1.1, 1.2, 1.3_


- [x] 2.2 Write RJ pattern validation function

  - Implement `validate_rj_pattern()` to verify format RJ-YYYY-NNNNN
  - Check for exactly 4 digits after "RJ-"
  - Verify hyphen separator
  - Check for exactly 5 digits after second hyphen
  - Return validation result (success/failure)
  - _Requirements: 1.4_

- [ ]* 2.3 Write unit tests for pattern matching
  - Test valid patterns (RJ-2024-12345, RJ-2025-00001)
  - Test invalid patterns (RJ-24-123, RJ-ABCD-12345)
  - Test multiple patterns in single content
  - Test content with no pattern
  - _Requirements: 1.1, 1.2, 1.3, 1.4, 1.5_

- [x] 3. Implement file operations module









  - _Requirements: 2.1, 2.2, 2.3, 2.4, 2.5, 4.3_

- [x] 3.1 Write file existence check function


  - Implement `file_exists()` using Windows API or stat
  - Return boolean result for file existence
  - Handle access permission errors
  - _Requirements: 2.2_

- [x] 3.2 Write unique filename generator

  - Implement `generate_unique_name()` for conflict resolution
  - Append numeric suffix (_1, _2, etc.) when file exists
  - Preserve .txt extension
  - Ensure generated name fits within MAX_PATH
  - _Requirements: 2.2_


- [x] 3.3 Write file rename function

  - Implement `rename_file()` to perform actual rename operation
  - Construct new filename from RJ pattern
  - Check for existing file and call unique name generator if needed
  - Use rename() system call for the operation
  - Update statistics counters based on result
  - Log success or failure with descriptive messages
  - _Requirements: 2.1, 2.2, 2.3, 2.4, 2.5, 4.3_

- [ ]* 3.4 Write unit tests for file operations
  - Test successful rename
  - Test conflict resolution with existing files
  - Test permission errors
  - Test path length limits
  - _Requirements: 2.1, 2.2, 2.3, 2.4, 2.5_


- [x] 4. Implement file processing module







  - _Requirements: 1.1, 1.5, 2.1, 4.2, 4.3, 5.2, 5.3_

- [x] 4.1 Write file content reader function


  - Implement `read_file_content()` to read entire file into memory
  - Allocate buffer dynamically based on file size
  - Handle file open errors with descriptive messages
  - Return allocated content buffer or NULL on error
  - _Requirements: 1.1, 4.2_

- [x] 4.2 Write main file processing function


  - Implement `process_file()` to orchestrate file processing
  - Call read_file_content() to get file data
  - Call extract_rj_pattern() to find RJ number
  - If pattern found, call rename_file() to rename
  - If no pattern, increment skipped counter and log
  - Free allocated memory before returning
  - Update statistics based on outcome
  - Handle and log all errors appropriately
  - _Requirements: 1.1, 1.5, 2.1, 4.2, 4.3, 5.2, 5.3_

- [ ]* 4.3 Write unit tests for file processing
  - Test processing file with valid RJ pattern
  - Test processing file without RJ pattern
  - Test processing empty file
  - Test handling of read errors
  - _Requirements: 1.1, 1.5, 2.1, 4.2, 4.3_

- [x] 5. Implement directory traversal module




  - _Requirements: 3.1, 3.2, 3.3, 3.4, 4.4, 6.1, 6.2, 6.3, 6.4, 6.5_

- [x] 5.1 Write .txt file filter function


  - Implement `is_txt_file()` to check file extension
  - Return true only for files ending in .txt
  - Handle case-insensitive comparison for Windows
  - _Requirements: 3.1_

- [x] 5.2 Write recursive directory processing function

  - Implement `process_directory()` using Windows API (FindFirstFile, FindNextFile)
  - Enumerate all entries in directory
  - Filter for .txt files using is_txt_file()
  - Call process_file() for each .txt file
  - Recursively process subdirectories
  - Handle directory access errors gracefully and continue processing
  - Use Windows path separators (backslash)
  - Check path lengths against MAX_PATH and skip if exceeded
  - Clean up handles with FindClose()
  - _Requirements: 3.1, 3.2, 3.3, 3.4, 4.4, 6.1, 6.2, 6.3, 6.4, 6.5_

- [ ]* 5.3 Write integration tests for directory traversal
  - Test single directory with multiple files
  - Test nested directory structures
  - Test empty directories
  - Test mixed file types
  - _Requirements: 3.1, 3.2, 3.3, 3.4_

- [x] 6. Implement main entry point and program flow










  - _Requirements: 4.1, 4.4, 5.1, 5.4, 5.5_


- [x] 6.1 Write command-line argument validation

  - Check argc for exactly 2 arguments (program name + directory path)
  - Display usage message if arguments invalid
  - Validate that provided path exists and is a directory
  - Return appropriate error code for invalid arguments
  - _Requirements: 4.1, 4.4_


- [x] 6.2 Implement main function

  - Parse and validate command-line arguments
  - Initialize Statistics structure to zero
  - Display starting message with target directory
  - Call process_directory() with target path
  - Print final summary with statistics (total, renamed, skipped, errors)
  - Return appropriate exit code (0 for success, non-zero for errors)
  - _Requirements: 4.1, 4.4, 5.1, 5.4, 5.5_


- [x] 6.3 Add error handling and logging throughout

  - Ensure all error messages go to stderr
  - Ensure informational messages go to stdout
  - Use descriptive error messages with context
  - Include errno details where applicable using strerror()
  - _Requirements: 4.1, 4.2, 4.3, 4.4, 4.5, 5.2, 5.3_

- [ ]* 6.4 Write end-to-end integration tests
  - Create test directory structure with sample files
  - Run program against test directory
  - Verify correct files were renamed
  - Verify skipped files unchanged
  - Verify statistics are accurate
  - Test error scenarios (invalid path, no arguments)
  - _Requirements: All requirements_

- [x] 7. Create build configuration and documentation



  - _Requirements: All requirements_



- [x] 7.1 Write Makefile or build script
  - Add GCC compilation command with appropriate flags (-Wall -Wextra -std=c99)
  - Include debug build target with -g flag
  - Include release build target with -O2 optimization
  - Add clean target to remove compiled files
  - _Requirements: All requirements_

- [x] 7.2 Create README with usage instructions


  - Document command-line usage and syntax
  - Provide examples of running the program
  - Explain RJ pattern format
  - Document expected behavior and output
  - Include troubleshooting section
  - Add compilation instructions
  - _Requirements: All requirements_


- [x] 7.3 Set up testing directory structure

  - Create testing/ folder with sample subdirectories
  - Add sample .txt files with various RJ patterns
  - Include edge cases (no pattern, multiple patterns, duplicates)
  - Add files with invalid patterns for negative testing
  - Document expected outcomes for test files
  - _Requirements: All requirements_
