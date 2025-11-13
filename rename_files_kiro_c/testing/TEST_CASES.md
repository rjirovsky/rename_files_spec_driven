# Test Cases for File Renaming Utility

## Overview
This directory contains test files to verify the RJ-YYYY-NNNNN pattern extraction and file renaming functionality.

## Test Files

### Valid RJ Pattern Files
These files contain valid RJ patterns and should be renamed:

1. **RJ-2024-12345_1.txt** - Standard RJ pattern in content
2. **RJ-2024-11111_1.txt** - File with multiple RJ patterns (uses first one)
3. **RJ-2023-98765_1.txt** - RJ pattern at end of file content
4. **subfolder/RJ-2024-77777_1.txt** - Nested file with RJ pattern
5. **subfolder/deep_nested/RJ-2024-99999.txt** - Deeply nested file with RJ pattern

### Invalid Pattern Files
These files should be skipped (not renamed):

1. **invalid_number.txt** - Contains RJ-2024-123 (number too short)
2. **invalid_pattern.txt** - Contains RJ-24-12345 (year too short)
3. **invalid_year.txt** - Contains RJ-ABCD-12345 (non-numeric year)
4. **test_invalid_rj.txt** - Multiple invalid patterns
5. **no_pattern.txt** - No RJ pattern at all
6. **test_no_rj.txt** - No RJ pattern
7. **empty_file.txt** - Empty file
8. **subfolder/nested_no_pattern.txt** - Nested file without pattern

## Expected Behavior

### Pattern Format
- Valid: `RJ-YYYY-NNNNN` where YYYY is 4 digits and NNNNN is 5 digits
- Example: RJ-2024-12345

### Renaming Rules
1. Files are renamed to `RJ-YYYY-NNNNN.txt`
2. If target name exists, append `_1`, `_2`, etc.
3. Only the first RJ pattern in a file is used
4. Invalid patterns are ignored
5. Files without patterns are skipped

### Directory Processing
- Processes all subdirectories recursively
- Only `.txt` files are processed
- Original file content is preserved

## Running Tests

```bash
# Run the utility on the testing directory
rename_files.exe testing

# Expected output:
# - 5 files renamed
# - 8 files skipped
# - 0 errors
```

## Test Results
After running the utility, verify:
- ✅ Files with valid RJ patterns are renamed correctly
- ✅ Files with invalid patterns are skipped
- ✅ Conflict resolution works (appends _1, _2, etc.)
- ✅ Nested directories are processed
- ✅ File contents remain unchanged
