# Technology Stack

## Build System & Tools
- Windows Batch (.bat) scripting
- Windows Command Prompt (cmd.exe)
- File system operations using native Windows commands
- Text processing with findstr and for loops

## Tech Stack
- Language: Windows Batch Script
- Platform: Windows OS
- File Operations: Native Windows commands (dir, ren, findstr)
- Text Processing: Batch string manipulation and regex patterns
- Testing: Manual testing with sample file structures

## Common Commands

```batch
# Run the file renamer utility
rename_files.bat

# Test the utility with sample data
cd testing
..\rename_files.bat

# View help or usage information
rename_files.bat /?
```

## Development Guidelines
- Use clear variable names with descriptive prefixes
- Include error handling for file operations
- Provide user feedback during processing
- Use comments to explain complex batch logic
- Test with various file structures and edge cases
- Handle special characters in filenames safely
- Use delayed expansion for variable manipulation
- Implement proper exit codes for success/failure

## Batch Script Best Practices
- Use `@echo off` to reduce output noise
- Enable delayed expansion with `setlocal enabledelayedexpansion`
- Quote file paths to handle spaces
- Use `if exist` checks before file operations
- Implement proper error handling with `if errorlevel`
- Provide clear status messages to users