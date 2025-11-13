# Design Document

## Overview

The File Renaming Utility is a single-executable C program that processes directories recursively to rename .txt files based on RJ numbers found in their content. The design emphasizes simplicity, safety, and clear error reporting while adhering to Windows file system conventions.

## Architecture

The system follows a modular, procedural design with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────┐
│                    Main Entry Point                      │
│              (Argument validation & setup)               │
└────────────────────┬────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────┐
│              Directory Traversal Module                  │
│         (Recursive directory processing)                 │
└────────────────────┬────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────┐
│               File Processing Module                     │
│    (Read file, extract pattern, rename operation)       │
└─────┬──────────────────────────────────────┬────────────┘
      │                                      │
      ▼                                      ▼
┌──────────────────┐              ┌──────────────────────┐
│ Pattern Matching │              │  File Operations     │
│     Module       │              │      Module          │
│ (RJ-YYYY-NNNNN)  │              │ (Rename with safety) │
└──────────────────┘              └──────────────────────┘
```

## Components and Interfaces

### 1. Main Entry Point

**Responsibility:** Command-line argument parsing and program initialization

**Functions:**
```c
int main(int argc, char *argv[]);
```

**Behavior:**
- Validates command-line arguments (expects exactly one directory path)
- Initializes statistics counters
- Calls directory traversal module
- Prints final summary
- Returns appropriate exit code

### 2. Directory Traversal Module

**Responsibility:** Recursively traverse directories and identify .txt files

**Functions:**
```c
int process_directory(const char *dir_path, Statistics *stats);
static int is_txt_file(const char *filename);
```

**Behavior:**
- Uses Windows API (`FindFirstFile`, `FindNextFile`) for directory traversal
- Filters for .txt files only
- Recursively processes subdirectories
- Passes each .txt file to the file processing module
- Handles directory access errors gracefully

**Windows API Usage:**
- `WIN32_FIND_DATA` structure for file information
- `FindFirstFile()` to start directory enumeration
- `FindNextFile()` to iterate through entries
- `FindClose()` to clean up handles

### 3. File Processing Module

**Responsibility:** Read file content, extract RJ pattern, and perform rename

**Functions:**
```c
int process_file(const char *filepath, Statistics *stats);
static char* read_file_content(const char *filepath);
```

**Behavior:**
- Opens and reads entire file content into memory
- Calls pattern matching module to extract RJ number
- If pattern found, calls file operations module to rename
- Updates statistics counters
- Frees allocated memory
- Logs all operations (success, skip, error)

**Error Handling:**
- File read failures (permissions, disk errors)
- Memory allocation failures
- Invalid file paths

### 4. Pattern Matching Module

**Responsibility:** Extract and validate RJ-YYYY-NNNNN patterns from text

**Functions:**
```c
int extract_rj_pattern(const char *content, char *output, size_t output_size);
static int validate_rj_pattern(const char *pattern);
```

**Behavior:**
- Searches for "RJ-" prefix in content
- Validates format: RJ-YYYY-NNNNN (4 digits, hyphen, 5 digits)
- Returns first valid occurrence
- Copies validated pattern to output buffer
- Returns 0 on success, -1 if no valid pattern found

**Pattern Validation:**
```c
// Expected format: RJ-YYYY-NNNNN
// Example: RJ-2024-12345
- Check for "RJ-" prefix
- Verify next 4 characters are digits (year)
- Verify hyphen separator
- Verify next 5 characters are digits (number)
- Ensure pattern ends (not part of longer string)
```

### 5. File Operations Module

**Responsibility:** Safely rename files with conflict resolution

**Functions:**
```c
int rename_file(const char *old_path, const char *new_name, Statistics *stats);
static int file_exists(const char *filepath);
static void generate_unique_name(const char *base_name, char *output, size_t output_size);
```

**Behavior:**
- Constructs new filename from RJ pattern
- Checks if target filename already exists
- If conflict, appends numeric suffix (_1, _2, etc.)
- Performs rename operation using `rename()` function
- Logs success or failure with details

**Conflict Resolution:**
```c
// If RJ-2024-12345.txt exists:
// Try RJ-2024-12345_1.txt
// Try RJ-2024-12345_2.txt
// Continue until unique name found
```

## Data Models

### Statistics Structure
```c
typedef struct {
    int total_files;      // Total .txt files encountered
    int renamed_files;    // Successfully renamed files
    int skipped_files;    // Files without valid RJ pattern
    int error_files;      // Files that encountered errors
} Statistics;
```

### File Path Buffer
```c
#define MAX_PATH_LENGTH 260  // Windows MAX_PATH
char filepath[MAX_PATH_LENGTH];
```

### Pattern Buffer
```c
#define RJ_PATTERN_LENGTH 16  // "RJ-YYYY-NNNNN" + null terminator
char rj_pattern[RJ_PATTERN_LENGTH];
```

## Error Handling

### Error Categories

1. **Fatal Errors** (terminate program):
   - Invalid command-line arguments
   - Target directory does not exist
   - Memory allocation failure for critical structures

2. **File-Level Errors** (log and continue):
   - Cannot read file (permissions)
   - Cannot rename file (permissions, disk full)
   - Path length exceeds MAX_PATH

3. **Informational** (not errors):
   - No RJ pattern found in file
   - File already has correct name

### Error Reporting Strategy

```c
// Error messages to stderr
fprintf(stderr, "Error: Cannot open directory '%s': %s\n", 
        dir_path, strerror(errno));

// Informational messages to stdout
printf("Renamed: %s -> %s\n", old_name, new_name);
printf("Skipped: %s (no RJ pattern found)\n", filename);
```

### Exit Codes
- `0`: Success (all operations completed)
- `1`: Invalid arguments
- `2`: Directory not found or inaccessible
- `3`: Critical error during processing

## Testing Strategy

### Unit Testing Approach

1. **Pattern Matching Tests**
   - Valid patterns: RJ-2024-12345, RJ-2025-00001
   - Invalid patterns: RJ-24-12345, RJ-2024-123, RJ-ABCD-12345
   - Edge cases: Multiple patterns, pattern at start/end of file
   - No pattern present

2. **File Operations Tests**
   - Successful rename
   - Conflict resolution (existing files)
   - Permission errors
   - Path length limits

3. **Directory Traversal Tests**
   - Single directory with multiple files
   - Nested directory structures
   - Empty directories
   - Mixed file types (.txt and others)

### Integration Testing

1. **End-to-End Scenarios**
   - Process test directory structure with known files
   - Verify correct files renamed
   - Verify skipped files unchanged
   - Verify error handling for problematic files

2. **Test Data Structure**
```
testing/
├── folder1/
│   ├── file_with_rj.txt          (contains RJ-2024-12345)
│   ├── file_without_rj.txt       (no RJ pattern)
│   └── subfolder1/
│       └── nested_file.txt       (contains RJ-2024-67890)
├── folder2/
│   ├── duplicate_rj.txt          (contains RJ-2024-12345)
│   └── multiple_patterns.txt     (contains RJ-2024-11111 and RJ-2024-22222)
└── edge_cases/
    ├── empty_file.txt
    ├── invalid_pattern.txt       (contains RJ-24-123)
    └── already_named.txt         (already named RJ-2024-99999.txt)
```

### Manual Testing Checklist

- [ ] Run with valid directory path
- [ ] Run with invalid directory path
- [ ] Run with no arguments
- [ ] Run with too many arguments
- [ ] Process directory with read-only files
- [ ] Process directory with files already named correctly
- [ ] Verify summary statistics are accurate
- [ ] Check error messages are clear and helpful

## Performance Considerations

- **Memory Usage:** Read entire file content into memory (acceptable for typical text files < 1MB)
- **File I/O:** Minimize file operations by reading once per file
- **Directory Traversal:** Use Windows API for efficient directory enumeration
- **String Operations:** Use safe string functions (strncpy, snprintf) to prevent buffer overflows

## Security Considerations

- **Buffer Overflow Prevention:** Use bounded string operations
- **Path Traversal:** Validate paths don't escape intended directory
- **Input Validation:** Sanitize all user inputs
- **File Permissions:** Respect Windows file system permissions
- **Error Information:** Don't expose sensitive system information in error messages

## Future Enhancements (Out of Scope)

- Support for additional file extensions beyond .txt
- Configuration file for custom pattern formats
- Dry-run mode to preview changes
- Undo functionality
- Parallel processing for large directory structures
- GUI interface
