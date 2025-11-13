# Makefile for File Renaming Utility
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
DEBUG_FLAGS = -g
RELEASE_FLAGS = -O2

# Target executable
TARGET = rename_files.exe

# Source files
SOURCES = rename_files.c

# Default target - release build
all: release

# Release build with optimization
release: $(SOURCES)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o $(TARGET) $(SOURCES)
	@echo "Release build complete: $(TARGET)"

# Debug build with debugging symbols
debug: $(SOURCES)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o $(TARGET) $(SOURCES)
	@echo "Debug build complete: $(TARGET)"

# Clean build artifacts
clean:
	del /Q $(TARGET) 2>nul || echo "No files to clean"

# Help target
help:
	@echo "Available targets:"
	@echo "  make          - Build release version (default)"
	@echo "  make release  - Build optimized release version"
	@echo "  make debug    - Build debug version with symbols"
	@echo "  make clean    - Remove compiled files"
	@echo "  make help     - Show this help message"

.PHONY: all release debug clean help
