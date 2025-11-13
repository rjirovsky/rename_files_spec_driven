# Technology Stack

## Build System

- Not yet configured

## Languages & Frameworks

- C (C99 or C11 standard)
- Windows API for file operations

## Compiler

- GCC (MinGW for Windows) or MSVC
- Target: Windows executable (.exe)

## Common Commands

### Building
```bash
# Using GCC/MinGW
gcc -Wall -Wextra -std=c99 -o rename_files.exe rename_files.c

# With optimization
gcc -Wall -Wextra -std=c99 -O2 -o rename_files.exe rename_files.c

# Debug build
gcc -Wall -Wextra -g -std=c99 -o rename_files.exe rename_files.c
```

### Testing
```bash
# Run with test directory
rename_files.exe testing/

# Test specific folder
rename_files.exe testing/folder1/
```

## Code Style Guidelines

- Use consistent indentation (4 spaces)
- Follow K&R or consistent brace style
- Use meaningful variable and function names
- Keep functions focused and under 50 lines when possible
- Always check return values for file operations
- Use `const` for read-only parameters

## Memory Management

- Always free allocated memory before exit
- Initialize pointers to NULL
- Check for NULL after malloc/calloc
- Clean up file handles with fclose()
- Use RAII-style patterns where possible (open/process/close in same scope)

## Error Handling

- Check all file operation return values
- Provide meaningful error messages
- Use errno for system error details
- Handle edge cases (permissions, disk full, invalid paths)
- Never silently fail

## Windows-Specific Considerations

- Use Windows path separators (backslash)
- Handle long file paths (MAX_PATH limitations)
- Consider file permissions and access rights
- Use appropriate Windows API functions for directory traversal
- Handle Unicode filenames if needed

## Recommended Compiler Flags

Development:
```bash
gcc -Wall -Wextra -Werror -std=c99 -pedantic -O2
```

Debugging:
```bash
gcc -Wall -Wextra -g -std=c99 -pedantic
```
