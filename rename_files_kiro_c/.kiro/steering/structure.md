# Project Structure

## Organization Principles
- Keep compiled files in a separate /bin directory
- Separate testing data from production scripts
- Use clear, descriptive naming for all files
- Maintain organized folder hierarchy for test scenarios

## Standard Directories
```
/
├── rename_files.exe   # Main utility binary
├── README.md          # Usage instructions and documentation
├── testing/           # Test environment with sample data
│   ├── folder1/       # Sample directory structure
│   │   ├── subfolder1/
│   │   └── subfolder2/
│   ├── folder2/       # Additional test directories
│   │   └── nested/
│   └── sample_files/  # Various test files with different patterns
└── .kiro/            # Kiro configuration and steering
    ├── specs/        # Feature specifications
    └── steering/     # AI assistant guidance files
```

## File Naming Conventions
- Use descriptive names for batch scripts (rename_files.bat)
- Test files should represent realistic scenarios
- Use underscores for batch script names (Windows convention)
- Sample files should demonstrate various RJ number patterns

## Code Organization
- Main logic in primary .c file
- Include .h files for function declarations
- Use .bat file for Windows compatibility

## C File Structure

### Source Organization
```c
// 1. System includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 2. Local includes (if any)
#include "rename_files.h"

// 3. Constants and macros
#define MAX_PATH_LENGTH 260
#define RJ_PATTERN "RJ-"

// 4. Type definitions
typedef struct {
    char old_name[MAX_PATH_LENGTH];
    char new_name[MAX_PATH_LENGTH];
} FileRename;

// 5. Static function declarations (internal helpers)
static int extract_rj_number(const char *content, char *output);
static int is_valid_rj_pattern(const char *pattern);

// 6. Public function implementations
// 7. Static function implementations
```

### Function Guidelines
- Use clear, descriptive function names (e.g., `extract_rj_number`, `process_directory`)
- Keep functions focused on a single task
- Separate file processing from pattern matching logic
- Use static for internal helper functions
- Return meaningful error codes (0 for success, non-zero for errors)
- Document function parameters and return values

### Error Handling Pattern
```c
FILE *fp = fopen(filename, "r");
if (fp == NULL) {
    fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
    return -1;
}

// Process file...

fclose(fp);
return 0;
```

### Memory Management
- Free all allocated memory before function returns
- Use cleanup labels for error paths
- Initialize all pointers to NULL
- Check malloc/calloc return values

### Naming Conventions
- Functions: lowercase with underscores (e.g., `process_file()`)
- Constants/Macros: UPPERCASE with underscores (e.g., `MAX_BUFFER_SIZE`)
- Variables: lowercase with underscores (e.g., `file_count`)
- Structs: lowercase with underscores (e.g., `struct file_info`)

## Test Environment Structure
- Create realistic folder hierarchies
- Include files with and without RJ numbers
- Test various RJ-YYYY-NNNNN patterns (different years, numbers)
- Include edge cases (multiple patterns, malformed patterns)
- Provide files with different extensions for negative testing

## Documentation
- Include usage examples in README.md
- Document expected file patterns and formats
- Provide troubleshooting guidance
- Include sample before/after scenarios