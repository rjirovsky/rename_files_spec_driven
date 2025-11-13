# Product Overview

A Windows executable utility for automated file renaming based on RJ numbers.

## Purpose
- Automatically rename .txt files by extracting RJ numbers from their content
- Process files recursively through directory structures
- Provide safe, reliable file operations without data loss

## Target Users
- Test engineers managing test results files
- System administrators handling bulk file operations

## Key Features
- Pattern-based file renaming using RJ-YYYY-NNNNN format
- Recursive directory processing
- Safe file handling with error management

## Goals
- Maintain data integrity during file operations
- Provide clear feedback on processing results
- Handle edge cases gracefully
- Follow Windows batch scripting best practices

## Context
When working on this project, consider:
- Windows file system limitations and permissions
- Error handling and robustness
- User experience for command-line tools
- Testing with various file structures and content patterns

## C Programming Principles for This Project

- Write portable C code where possible
- Use standard C library functions (stdio.h, stdlib.h, string.h)
- Implement proper error handling for all file operations
- Validate input parameters and file paths
- Use safe string functions (strncpy, snprintf) to prevent buffer overflows
- Document complex logic with comments
- Test edge cases thoroughly (empty files, missing patterns, permission errors)