# File Renaming Utility

A Windows command-line utility that automatically renames `.txt` files based on RJ numbers found within their content.

## Overview

The File Renaming Utility scans text files for RJ identifiers in the format `RJ-YYYY-NNNNN` and renames the files accordingly. It processes directories recursively, making it easy to organize large collections of test result files.

## RJ Pattern Format

The utility searches for RJ numbers in the following format:

```
RJ-YYYY-NNNNN
```

Where:
- `RJ-` is the fixed prefix
- `YYYY` is a four-digit year (e.g., 2024)
- `-` is a hyphen separator
- `NNNNN` is a five-digit number (e.g., 12345)

Examples of valid patterns:
- `RJ-2024-12345`
- `RJ-2025-00001`
- `RJ-2023-98765`

## Compilation

### Prerequisites

- GCC compiler (MinGW for Windows) or MSVC
- Windows operating system

### Build Instructions

Using the provided Makefile:

```bash
# Build release version (optimized)
make

# Or explicitly:
make release

# Build debug version (with debugging symbols)
make debug

# Clean build artifacts
make clean
```

Manual compilation:

```bash
# Release build
gcc -Wall -Wextra -std=c99 -O2 -o rename_files.exe rename_files.c

# Debug build
gcc -Wall -Wextra -std=c99 -g -o rename_files.exe rename_files.c
```

## Usage

### Command-Line Syntax

```bash
rename_files.exe <directory_path>
```

### Parameters

- `<directory_path>`: Path to the directory containing `.txt` files to process (required)
  - Can be absolute or relative path
  - Supports both forward slashes (/) and backslashes (\)

### Examples

Process files in the current directory:
```bash
rename_files.exe .
```

Process files in a specific directory:
```bash
rename_files.exe C:\TestResults
```

Process files in a relative subdirectory:
```bash
rename_files.exe testing\folder1
```

## Behavior

### File Processing

1. The utility recursively scans all subdirectories within the specified path
2. Only `.txt` files are processed
3. Each file's content is read to search for an RJ pattern
4. If a valid pattern is found, the file is renamed to `RJ-YYYY-NNNNN.txt`
5. If multiple patterns exist in a file, only the first occurrence is used

### Conflict Resolution

If a file with the target name already exists, the utility automatically appends a numeric suffix:

```
Original file: document.txt (contains RJ-2024-12345)
Target name: RJ-2024-12345.txt (already exists)
New name: RJ-2024-12345_1.txt

If RJ-2024-12345_1.txt also exists:
New name: RJ-2024-12345_2.txt
```

### Files That Are Skipped

Files are skipped (not renamed) in the following cases:
- No RJ pattern found in the file content
- File contains an invalid RJ pattern (wrong format)
- File is empty
- File cannot be read due to permissions

## Output

### Success Messages

```
Starting file renaming utility...
Target directory: C:\TestResults

Renamed: old_document.txt -> RJ-2024-12345.txt
Renamed: test_results.txt -> RJ-2024-67890.txt
Skipped: notes.txt (no RJ pattern found)

Summary:
Total files processed: 3
Successfully renamed: 2
Skipped: 1
Errors: 0
```

### Error Messages

```
Error: Directory 'C:\InvalidPath' does not exist
Error: Cannot read file 'locked.txt': Permission denied
Error: Path length exceeds maximum allowed (260 characters)
```

## Exit Codes

- `0`: Success - all operations completed
- `1`: Invalid command-line arguments
- `2`: Directory not found or inaccessible
- `3`: Critical error during processing

## Troubleshooting

### Problem: "Directory does not exist"

**Solution:** Verify the directory path is correct and accessible. Use quotes if the path contains spaces:
```bash
rename_files.exe "C:\Test Results\Folder"
```

### Problem: "Permission denied" errors

**Solution:** 
- Ensure you have read/write permissions for the directory
- Close any programs that might have the files open
- Run the command prompt as Administrator if necessary

### Problem: Files not being renamed

**Possible causes:**
- File doesn't contain a valid RJ pattern
- RJ pattern format is incorrect (must be RJ-YYYY-NNNNN)
- File is not a `.txt` file
- File is locked by another process

**Solution:** Check the file content and verify it contains a properly formatted RJ pattern.

### Problem: "Path length exceeds maximum"

**Solution:** Windows has a 260-character path limit. Move files to a location with a shorter path or use a directory closer to the root.

### Problem: Unexpected file names after renaming

**Cause:** Multiple RJ patterns in the same file

**Solution:** The utility uses the first RJ pattern found. Edit the file to remove unwanted patterns if necessary.

## Testing

A `testing/` directory is provided with sample files to verify the utility works correctly:

```bash
# Test the utility
rename_files.exe testing
```

The testing directory includes:
- Files with valid RJ patterns
- Files without RJ patterns
- Files with invalid patterns
- Nested subdirectories
- Duplicate RJ numbers (for conflict resolution testing)

## Safety Features

- Original files are never deleted, only renamed
- If a rename operation fails, the original file remains unchanged
- All file operations are logged for transparency
- Conflict resolution prevents overwriting existing files
- Comprehensive error handling ensures robustness

## Limitations

- Only processes `.txt` files
- Maximum path length: 260 characters (Windows MAX_PATH)
- Reads entire file content into memory (suitable for typical text files)
- Windows-only (uses Windows API for directory traversal)

## License

This utility is provided as-is for internal use.

## Support

For issues or questions, refer to the design and requirements documents in `.kiro/specs/file-renaming-utility/`.
