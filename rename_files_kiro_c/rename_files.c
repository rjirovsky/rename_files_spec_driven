/*
 * File Renaming Utility
 * Automatically renames .txt files based on RJ-YYYY-NNNNN patterns found in their content
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <windows.h>

// Constants
#define MAX_PATH_LENGTH 260
#define RJ_PATTERN_LENGTH 14  // "RJ-YYYY-NNNNN" (13 chars) + null terminator

// Statistics structure for tracking file operations
typedef struct {
    int total_files;      // Total .txt files encountered
    int renamed_files;    // Successfully renamed files
    int skipped_files;    // Files without valid RJ pattern
    int error_files;      // Files that encountered errors
} Statistics;

// Function declarations
static int validate_rj_pattern(const char *pattern);
int extract_rj_pattern(const char *content, char *output, size_t output_size);
static int file_exists(const char *filepath);
static void generate_unique_name(const char *base_name, char *output, size_t output_size);
int rename_file(const char *old_path, const char *new_name, Statistics *stats);
static char* read_file_content(const char *filepath);
int process_file(const char *filepath, Statistics *stats);
static int is_txt_file(const char *filename);
int process_directory(const char *dir_path, Statistics *stats);
static int validate_arguments(int argc, char *argv[]);

// Pattern Matching Module Implementation

/**
 * Validates an RJ pattern to ensure it matches the format RJ-YYYY-NNNNN
 * @param pattern The pattern string to validate
 * @return 1 if valid, 0 if invalid
 */
static int validate_rj_pattern(const char *pattern) {
    // RJ_PATTERN_LENGTH includes null terminator, so actual pattern is 13 chars
    const size_t PATTERN_LEN = RJ_PATTERN_LENGTH - 1;
    
    if (pattern == NULL || strlen(pattern) < PATTERN_LEN) {
        return 0;
    }
    
    // Check for "RJ-" prefix
    if (pattern[0] != 'R' || pattern[1] != 'J' || pattern[2] != '-') {
        return 0;
    }
    
    // Check for exactly 4 digits after "RJ-" (year)
    for (int i = 3; i < 7; i++) {
        if (pattern[i] < '0' || pattern[i] > '9') {
            return 0;
        }
    }
    
    // Verify hyphen separator
    if (pattern[7] != '-') {
        return 0;
    }
    
    // Check for exactly 5 digits after second hyphen
    for (int i = 8; i < 13; i++) {
        if (pattern[i] < '0' || pattern[i] > '9') {
            return 0;
        }
    }
    
    // Ensure pattern ends properly (not part of a longer alphanumeric string)
    if (pattern[13] != '\0' && 
        ((pattern[13] >= '0' && pattern[13] <= '9') ||
         (pattern[13] >= 'A' && pattern[13] <= 'Z') ||
         (pattern[13] >= 'a' && pattern[13] <= 'z'))) {
        return 0;
    }
    
    return 1;
}

/**
 * Extracts the first valid RJ-YYYY-NNNNN pattern from file content
 * @param content The file content to search
 * @param output Buffer to store the extracted pattern
 * @param output_size Size of the output buffer
 * @return 0 on success, -1 if no valid pattern found
 */
int extract_rj_pattern(const char *content, char *output, size_t output_size) {
    if (content == NULL || output == NULL || output_size < RJ_PATTERN_LENGTH) {
        return -1;
    }
    
    const char *ptr = content;
    const size_t PATTERN_LEN = RJ_PATTERN_LENGTH - 1;  // Exclude null terminator
    
    // Search for "RJ-" prefix in content
    while ((ptr = strstr(ptr, "RJ-")) != NULL) {
        // Check if we have enough characters for a complete pattern
        if (strlen(ptr) >= PATTERN_LEN) {
            char candidate[RJ_PATTERN_LENGTH];
            strncpy(candidate, ptr, PATTERN_LEN);
            candidate[PATTERN_LEN] = '\0';
            
            // Validate the extracted pattern
            if (validate_rj_pattern(candidate)) {
                // Copy validated pattern to output buffer
                strncpy(output, candidate, output_size - 1);
                output[output_size - 1] = '\0';
                return 0;
            }
        }
        
        // Move to next character to continue searching
        ptr++;
    }
    
    // No valid pattern found
    return -1;
}

// File Operations Module Implementation

/**
 * Checks if a file exists at the specified path
 * @param filepath The path to check
 * @return 1 if file exists, 0 if not or on error
 */
static int file_exists(const char *filepath) {
    if (filepath == NULL) {
        return 0;
    }
    
    DWORD attributes = GetFileAttributesA(filepath);
    
    // Check if the path exists and is not a directory
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        return 0;
    }
    
    // Return 0 if it's a directory, 1 if it's a file
    return (attributes & FILE_ATTRIBUTE_DIRECTORY) ? 0 : 1;
}

/**
 * Generates a unique filename by appending numeric suffix if needed
 * @param base_name The base filename (e.g., "RJ-2024-12345.txt")
 * @param output Buffer to store the unique filename
 * @param output_size Size of the output buffer
 */
static void generate_unique_name(const char *base_name, char *output, size_t output_size) {
    if (base_name == NULL || output == NULL || output_size == 0) {
        return;
    }
    
    // First, try the base name without any suffix
    strncpy(output, base_name, output_size - 1);
    output[output_size - 1] = '\0';
    
    if (!file_exists(output)) {
        return;  // Base name is available
    }
    
    // Extract the name without extension
    char name_without_ext[MAX_PATH_LENGTH];
    const char *ext_pos = strrchr(base_name, '.');
    
    if (ext_pos != NULL) {
        size_t name_len = ext_pos - base_name;
        if (name_len >= MAX_PATH_LENGTH) {
            name_len = MAX_PATH_LENGTH - 1;
        }
        strncpy(name_without_ext, base_name, name_len);
        name_without_ext[name_len] = '\0';
    } else {
        strncpy(name_without_ext, base_name, MAX_PATH_LENGTH - 1);
        name_without_ext[MAX_PATH_LENGTH - 1] = '\0';
        ext_pos = "";  // No extension
    }
    
    // Try appending numeric suffixes until we find a unique name
    int suffix = 1;
    while (suffix < 10000) {  // Reasonable limit to prevent infinite loop
        snprintf(output, output_size, "%s_%d%s", name_without_ext, suffix, ext_pos);
        
        if (!file_exists(output)) {
            return;  // Found a unique name
        }
        
        suffix++;
    }
    
    // If we couldn't find a unique name, just use the last attempt
    // This shouldn't happen in practice
}

/**
 * Renames a file based on the RJ pattern
 * @param old_path The current file path
 * @param new_name The new filename (RJ-YYYY-NNNNN.txt)
 * @param stats Statistics structure to update
 * @return 0 on success, -1 on failure
 */
int rename_file(const char *old_path, const char *new_name, Statistics *stats) {
    if (old_path == NULL || new_name == NULL || stats == NULL) {
        fprintf(stderr, "Error: Invalid parameters to rename_file\n");
        if (stats != NULL) {
            stats->error_files++;
        }
        return -1;
    }
    
    // Extract directory path from old_path
    char new_path[MAX_PATH_LENGTH];
    const char *last_separator = strrchr(old_path, '\\');
    
    if (last_separator == NULL) {
        last_separator = strrchr(old_path, '/');
    }
    
    if (last_separator != NULL) {
        // File is in a subdirectory
        size_t dir_len = last_separator - old_path + 1;
        if (dir_len >= MAX_PATH_LENGTH - strlen(new_name) - 1) {
            fprintf(stderr, "Error: Path too long for '%s'\n", old_path);
            stats->error_files++;
            return -1;
        }
        
        strncpy(new_path, old_path, dir_len);
        new_path[dir_len] = '\0';
        strncat(new_path, new_name, MAX_PATH_LENGTH - dir_len - 1);
    } else {
        // File is in current directory
        strncpy(new_path, new_name, MAX_PATH_LENGTH - 1);
        new_path[MAX_PATH_LENGTH - 1] = '\0';
    }
    
    // Check if target file already exists and generate unique name if needed
    char final_path[MAX_PATH_LENGTH];
    generate_unique_name(new_path, final_path, MAX_PATH_LENGTH);
    
    // Perform the rename operation
    if (rename(old_path, final_path) != 0) {
        fprintf(stderr, "Error: Cannot rename '%s' to '%s': %s\n", 
                old_path, final_path, strerror(errno));
        stats->error_files++;
        return -1;
    }
    
    // Log success
    const char *old_filename = strrchr(old_path, '\\');
    if (old_filename == NULL) {
        old_filename = strrchr(old_path, '/');
    }
    old_filename = (old_filename != NULL) ? old_filename + 1 : old_path;
    
    const char *new_filename = strrchr(final_path, '\\');
    if (new_filename == NULL) {
        new_filename = strrchr(final_path, '/');
    }
    new_filename = (new_filename != NULL) ? new_filename + 1 : final_path;
    
    printf("Renamed: %s -> %s\n", old_filename, new_filename);
    stats->renamed_files++;
    
    return 0;
}

// File Processing Module Implementation

/**
 * Reads entire file content into memory
 * @param filepath The path to the file to read
 * @return Allocated buffer containing file content, or NULL on error
 *         Caller is responsible for freeing the returned buffer
 */
static char* read_file_content(const char *filepath) {
    if (filepath == NULL) {
        fprintf(stderr, "Error: Invalid filepath parameter\n");
        return NULL;
    }
    
    // Open file for reading
    FILE *fp = fopen(filepath, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s': %s\n", filepath, strerror(errno));
        return NULL;
    }
    
    // Get file size
    if (fseek(fp, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error: Cannot seek in file '%s': %s\n", filepath, strerror(errno));
        fclose(fp);
        return NULL;
    }
    
    long file_size = ftell(fp);
    if (file_size < 0) {
        fprintf(stderr, "Error: Cannot determine size of file '%s': %s\n", filepath, strerror(errno));
        fclose(fp);
        return NULL;
    }
    
    if (fseek(fp, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Error: Cannot seek to beginning of file '%s': %s\n", filepath, strerror(errno));
        fclose(fp);
        return NULL;
    }
    
    // Allocate buffer for file content (plus null terminator)
    char *content = (char*)malloc(file_size + 1);
    if (content == NULL) {
        fprintf(stderr, "Error: Cannot allocate memory for file '%s' (%ld bytes)\n", filepath, file_size + 1);
        fclose(fp);
        return NULL;
    }
    
    // Read file content
    size_t bytes_read = fread(content, 1, file_size, fp);
    if (bytes_read != (size_t)file_size) {
        if (ferror(fp)) {
            fprintf(stderr, "Error: Cannot read file '%s': %s\n", filepath, strerror(errno));
        } else {
            fprintf(stderr, "Error: Incomplete read of file '%s'\n", filepath);
        }
        free(content);
        fclose(fp);
        return NULL;
    }
    
    // Null-terminate the content
    content[file_size] = '\0';
    
    fclose(fp);
    return content;
}

/**
 * Processes a single file: reads content, extracts RJ pattern, and renames if found
 * @param filepath The path to the file to process
 * @param stats Statistics structure to update
 * @return 0 on success, -1 on error
 */
int process_file(const char *filepath, Statistics *stats) {
    if (filepath == NULL || stats == NULL) {
        fprintf(stderr, "Error: Invalid parameters to process_file\n");
        if (stats != NULL) {
            stats->error_files++;
        }
        return -1;
    }
    
    // Read file content
    char *content = read_file_content(filepath);
    if (content == NULL) {
        // Error already logged by read_file_content
        stats->error_files++;
        return -1;
    }
    
    // Extract RJ pattern from content
    char rj_pattern[RJ_PATTERN_LENGTH];
    int extract_result = extract_rj_pattern(content, rj_pattern, RJ_PATTERN_LENGTH);
    
    if (extract_result == 0) {
        // Pattern found - construct new filename
        char new_filename[MAX_PATH_LENGTH];
        snprintf(new_filename, MAX_PATH_LENGTH, "%s.txt", rj_pattern);
        
        // Attempt to rename the file
        int rename_result = rename_file(filepath, new_filename, stats);
        
        free(content);
        return rename_result;
    } else {
        // No valid RJ pattern found - skip this file
        const char *filename = strrchr(filepath, '\\');
        if (filename == NULL) {
            filename = strrchr(filepath, '/');
        }
        filename = (filename != NULL) ? filename + 1 : filepath;
        
        printf("Skipped: %s (no RJ pattern found)\n", filename);
        stats->skipped_files++;
        
        free(content);
        return 0;
    }
}

// Directory Traversal Module Implementation

/**
 * Checks if a filename has a .txt extension (case-insensitive)
 * @param filename The filename to check
 * @return 1 if file ends with .txt, 0 otherwise
 */
static int is_txt_file(const char *filename) {
    if (filename == NULL) {
        return 0;
    }
    
    size_t len = strlen(filename);
    
    // Need at least 5 characters for "a.txt"
    if (len < 5) {
        return 0;
    }
    
    // Get the last 4 characters
    const char *ext = filename + len - 4;
    
    // Case-insensitive comparison for Windows
    if (_stricmp(ext, ".txt") == 0) {
        return 1;
    }
    
    return 0;
}

/**
 * Recursively processes a directory and all its subdirectories
 * @param dir_path The directory path to process
 * @param stats Statistics structure to update
 * @return 0 on success, -1 on error
 */
int process_directory(const char *dir_path, Statistics *stats) {
    if (dir_path == NULL || stats == NULL) {
        fprintf(stderr, "Error: Invalid parameters to process_directory\n");
        return -1;
    }
    
    // Construct search pattern (directory\*)
    char search_pattern[MAX_PATH_LENGTH];
    size_t dir_len = strlen(dir_path);
    
    if (dir_len >= MAX_PATH_LENGTH - 3) {
        fprintf(stderr, "Error: Directory path too long: %s\n", dir_path);
        return -1;
    }
    
    // Copy directory path and ensure it ends with backslash
    strncpy(search_pattern, dir_path, MAX_PATH_LENGTH - 3);
    search_pattern[MAX_PATH_LENGTH - 3] = '\0';
    
    if (dir_len > 0 && search_pattern[dir_len - 1] != '\\' && search_pattern[dir_len - 1] != '/') {
        strncat(search_pattern, "\\", MAX_PATH_LENGTH - strlen(search_pattern) - 1);
    }
    
    strncat(search_pattern, "*", MAX_PATH_LENGTH - strlen(search_pattern) - 1);
    
    // Start directory enumeration
    WIN32_FIND_DATAA find_data;
    HANDLE hFind = FindFirstFileA(search_pattern, &find_data);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND) {
            // Empty directory is not an error
            return 0;
        }
        fprintf(stderr, "Error: Cannot open directory '%s': Error code %lu\n", dir_path, error);
        return -1;
    }
    
    // Process each entry in the directory
    do {
        // Skip "." and ".." entries
        if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0) {
            continue;
        }
        
        // Construct full path for this entry
        char full_path[MAX_PATH_LENGTH];
        size_t base_len = strlen(dir_path);
        
        // Check if path will exceed MAX_PATH
        if (base_len + strlen(find_data.cFileName) + 2 >= MAX_PATH_LENGTH) {
            fprintf(stderr, "Error: Path too long, skipping: %s\\%s\n", dir_path, find_data.cFileName);
            stats->error_files++;
            continue;
        }
        
        // Build full path
        strncpy(full_path, dir_path, MAX_PATH_LENGTH - 1);
        full_path[MAX_PATH_LENGTH - 1] = '\0';
        
        if (base_len > 0 && full_path[base_len - 1] != '\\' && full_path[base_len - 1] != '/') {
            strncat(full_path, "\\", MAX_PATH_LENGTH - strlen(full_path) - 1);
        }
        
        strncat(full_path, find_data.cFileName, MAX_PATH_LENGTH - strlen(full_path) - 1);
        
        // Check if this is a directory
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // Recursively process subdirectory
            process_directory(full_path, stats);
        } else {
            // Check if this is a .txt file
            if (is_txt_file(find_data.cFileName)) {
                stats->total_files++;
                process_file(full_path, stats);
            }
        }
        
    } while (FindNextFileA(hFind, &find_data) != 0);
    
    // Check if loop ended due to error or normal completion
    DWORD error = GetLastError();
    if (error != ERROR_NO_MORE_FILES) {
        fprintf(stderr, "Error: Error reading directory '%s': Error code %lu\n", dir_path, error);
        FindClose(hFind);
        return -1;
    }
    
    // Clean up handle
    FindClose(hFind);
    
    return 0;
}

// Main Entry Point Implementation

/**
 * Validates command-line arguments
 * @param argc Argument count
 * @param argv Argument values
 * @return 0 if valid, non-zero error code if invalid
 */
static int validate_arguments(int argc, char *argv[]) {
    // Check for exactly 2 arguments (program name + directory path)
    if (argc != 2) {
        fprintf(stderr, "Error: Invalid number of arguments\n");
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        fprintf(stderr, "\nDescription:\n");
        fprintf(stderr, "  Recursively processes .txt files in the specified directory,\n");
        fprintf(stderr, "  extracting RJ-YYYY-NNNNN patterns from file contents and renaming\n");
        fprintf(stderr, "  files accordingly.\n");
        fprintf(stderr, "\nExample:\n");
        fprintf(stderr, "  %s C:\\test_files\n", argv[0]);
        fprintf(stderr, "  %s .\\testing\n", argv[0]);
        return 1;
    }
    
    const char *dir_path = argv[1];
    
    // Validate that the provided path exists
    DWORD attributes = GetFileAttributesA(dir_path);
    
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        fprintf(stderr, "Error: Directory '%s' does not exist or cannot be accessed: %s\n", 
                dir_path, strerror(errno));
        return 2;
    }
    
    // Validate that it's a directory, not a file
    if (!(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        fprintf(stderr, "Error: '%s' is not a directory\n", dir_path);
        return 2;
    }
    
    return 0;
}

/**
 * Main entry point for the File Renaming Utility
 * @param argc Argument count
 * @param argv Argument values
 * @return 0 on success, non-zero on error
 */
int main(int argc, char *argv[]) {
    // Validate command-line arguments
    int validation_result = validate_arguments(argc, argv);
    if (validation_result != 0) {
        return validation_result;
    }
    
    const char *target_directory = argv[1];
    
    // Initialize statistics structure
    Statistics stats = {0, 0, 0, 0};
    
    // Display starting message
    printf("File Renaming Utility\n");
    printf("=====================\n");
    printf("Processing directory: %s\n\n", target_directory);
    
    // Process the directory recursively
    int process_result = process_directory(target_directory, &stats);
    
    // Print final summary
    printf("\n");
    printf("Processing Complete\n");
    printf("===================\n");
    printf("Total .txt files found: %d\n", stats.total_files);
    printf("Files renamed:          %d\n", stats.renamed_files);
    printf("Files skipped:          %d\n", stats.skipped_files);
    printf("Errors encountered:     %d\n", stats.error_files);
    
    // Return appropriate exit code
    if (process_result != 0) {
        fprintf(stderr, "\nWarning: Directory processing encountered errors\n");
        return 3;
    }
    
    if (stats.error_files > 0) {
        fprintf(stderr, "\nWarning: Some files could not be processed\n");
        return 3;
    }
    
    return 0;
}
