# Requirements Document

## Introduction

The File Renaming Utility is a Windows command-line executable that automatically renames text files based on RJ numbers found within their content. The system processes directories recursively, extracting RJ-YYYY-NNNNN patterns from file contents and renaming files accordingly while maintaining data integrity and providing clear feedback to users.

## Glossary

- **System**: The File Renaming Utility executable (rename_files.exe)
- **RJ Pattern**: RJ identifier in the format RJ-YYYY-NNNNN where YYYY is a four-digit year and NNNNN is a five-digit number
- **Source File**: A .txt file in the target directory that may contain an RJ Pattern
- **Target Directory**: The directory path provided as a command-line argument to process
- **File Content**: The text data contained within a Source File
- **Rename Operation**: The action of changing a Source File's name to match its extracted RJ Pattern

## Requirements

### Requirement 1

**User Story:** As a test engineer, I want the system to extract RJ numbers from file contents, so that files are automatically named according to their RJ identifiers.

#### Acceptance Criteria

1. WHEN the System reads a Source File, THE System SHALL scan the File Content for the RJ Pattern
2. IF the System finds an RJ Pattern in the File Content, THEN THE System SHALL extract the complete RJ-YYYY-NNNNN identifier
3. WHEN multiple RJ Patterns exist in a single Source File, THE System SHALL extract the first occurrence
4. THE System SHALL validate that the extracted pattern matches the format RJ-YYYY-NNNNN where YYYY is four digits and NNNNN is five digits
5. IF no valid RJ Pattern exists in the File Content, THEN THE System SHALL skip the Source File and continue processing

### Requirement 2

**User Story:** As a test engineer, I want files to be renamed based on extracted RJ numbers, so that file names reflect their content without manual intervention.

#### Acceptance Criteria

1. WHEN the System extracts a valid RJ Pattern, THE System SHALL rename the Source File to RJ-YYYY-NNNNN.txt
2. IF a file with the target name already exists in the same directory, THEN THE System SHALL append a numeric suffix starting with _1 to create a unique filename
3. THE System SHALL preserve the .txt file extension during the Rename Operation
4. THE System SHALL maintain the file in its original directory location during the Rename Operation
5. WHEN the Rename Operation completes successfully, THE System SHALL log the old and new filenames

### Requirement 3

**User Story:** As a test engineer, I want the system to process entire directory structures, so that I can rename multiple files across folders in a single operation.

#### Acceptance Criteria

1. WHEN the System receives a Target Directory as input, THE System SHALL process all .txt files in that directory
2. THE System SHALL recursively traverse all subdirectories within the Target Directory
3. THE System SHALL process each Source File independently without dependencies on other files
4. THE System SHALL continue processing remaining files if a single Rename Operation fails
5. WHEN processing completes, THE System SHALL report the total count of files processed and renamed

### Requirement 4

**User Story:** As a system administrator, I want comprehensive error handling, so that file operations are safe and failures are clearly communicated.

#### Acceptance Criteria

1. IF the Target Directory does not exist, THEN THE System SHALL display an error message and terminate with a non-zero exit code
2. IF the System cannot read a Source File due to permissions, THEN THE System SHALL log an error message and continue processing other files
3. IF a Rename Operation fails, THEN THE System SHALL log the specific error reason and preserve the original file unchanged
4. THE System SHALL validate the command-line argument before processing begins
5. WHEN any error occurs, THE System SHALL output descriptive error messages to stderr

### Requirement 5

**User Story:** As a test engineer, I want clear feedback during processing, so that I understand what the system is doing and can verify results.

#### Acceptance Criteria

1. WHEN processing begins, THE System SHALL display the Target Directory being processed
2. WHEN the System renames a file, THE System SHALL output a message showing the old name and new name
3. WHEN the System skips a file, THE System SHALL output a message indicating why the file was skipped
4. WHEN processing completes, THE System SHALL display a summary with counts of files processed, renamed, and skipped
5. THE System SHALL output all informational messages to stdout

### Requirement 6

**User Story:** As a system administrator, I want the utility to handle Windows-specific file system requirements, so that it operates reliably on Windows platforms.

#### Acceptance Criteria

1. THE System SHALL use Windows path separators (backslash) for all path operations
2. THE System SHALL handle file paths up to MAX_PATH length (260 characters)
3. THE System SHALL respect Windows file system permissions and access rights
4. THE System SHALL handle both absolute and relative path inputs for the Target Directory
5. IF the System encounters a path length exceeding MAX_PATH, THEN THE System SHALL log an error and skip that file
