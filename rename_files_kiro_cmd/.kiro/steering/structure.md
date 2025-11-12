# Project Structure

## Organization Principles
- Keep batch scripts at root level for easy execution
- Separate testing data from production scripts
- Use clear, descriptive naming for all files
- Maintain organized folder hierarchy for test scenarios

## Standard Directories
```
/
├── rename_files.bat    # Main utility script
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
- Sample files should demonstrate various RJ numbers patterns

## Batch Script Organization
- Main logic in primary .bat file
- Use subroutines (:label) for complex operations
- Group related functionality with clear comments
- Separate file processing from pattern matching logic
- Include error handling sections

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