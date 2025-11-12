# Requirements Document

## Introduction

A Windows batch script utility that automatically renames files based on RJ numbers found within their content. The system searches for specific RJ numbers  patterns and appends them to filenames for better organization and identification.

## Glossary

- **File Renamer Utility**: The batch script system that processes and renames files
- **RJ number**: A formatted identifier in the pattern RJ-YYYY-NNNNN where YYYY is year and NNNNN is a 5-digit sequence
- **Working Folder**: The directory where the batch script is executed, including all subdirectories
- **Target Files**: Files with .txt extension that contain RJ numbers

## Requirements

### Requirement 1

**User Story:** As a test administrator, I want to automatically rename files based on RJ numbers found in their content, so that I can easily identify and organize test-related documents.

#### Acceptance Criteria

1. WHEN the File Renamer Utility is executed, THE system SHALL search all .txt files in the working folder and subdirectories
2. WHEN a RJ number in format RJ-YYYY-NNNNN is found within a file, THE system SHALL rename the file to include the RJ number
3. THE system SHALL preserve the original filename and extension while appending the RJ number
4. THE system SHALL create new filenames in format `<original_name>_<rj_number>.<extension>`
5. IF multiple RJ numbers exist in a single file, THEN THE system SHALL use the first occurrence found

### Requirement 2

**User Story:** As a user, I want the utility to work recursively through all subdirectories, so that all relevant files are processed regardless of their location.

#### Acceptance Criteria

1. THE File Renamer Utility SHALL process files in the working folder and all subdirectories recursively
2. THE system SHALL maintain the original directory structure of processed files
3. WHEN processing subdirectories, THE system SHALL apply the same renaming logic consistently
4. THE system SHALL handle nested folder structures without depth limitations

### Requirement 3

**User Story:** As a user, I want the batch script to be safe and reliable, so that I don't accidentally lose or corrupt my files.

#### Acceptance Criteria

1. THE File Renamer Utility SHALL only rename files that contain valid RJ numbers
2. THE system SHALL preserve original file content during the renaming process
3. IF a file already has a RJ number in its filename, THEN THE system SHALL skip processing that file
4. THE system SHALL handle file access errors gracefully without terminating the entire process
5. THE system SHALL provide feedback on the number of files processed and renamed