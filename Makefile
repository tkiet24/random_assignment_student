# Makefile for Student Activity Assignment System
# Supports both GCC and MSVC through different targets

# Configuration
CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -O2
MSVC_FLAGS = /std:c++23 /W4 /O2 /EHsc

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Target executable
TARGET = StudentActivityAssignment
MSVC_TARGET = $(TARGET).exe

# Source files
SOURCES = $(SRC_DIR)/main.cpp

# Headers (for dependency tracking)
HEADERS = $(wildcard $(SRC_DIR)/**/*.h)

# Default target
.PHONY: all clean run setup msvc-build msvc-run

all: setup $(BIN_DIR)/$(TARGET)

# Setup directories
setup:
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	@copy students.txt $(BIN_DIR)\ >nul 2>&1
	@copy activities.txt $(BIN_DIR)\ >nul 2>&1

# GCC/Clang build
$(BIN_DIR)/$(TARGET): $(SOURCES) $(HEADERS) | setup
	$(CXX) $(CXXFLAGS) -I. $(SOURCES) -o $@

# MSVC build
msvc-build: setup
	cl $(MSVC_FLAGS) /I. $(SOURCES) /Fe:$(BIN_DIR)/$(MSVC_TARGET)

# Run with GCC build
run: $(BIN_DIR)/$(TARGET)
	@echo Running Student Activity Assignment System...
	@cd $(BIN_DIR) && ./$(TARGET)

# Run with MSVC build
msvc-run: msvc-build
	@echo Running Student Activity Assignment System...
	@cd $(BIN_DIR) && $(MSVC_TARGET)

# Clean build artifacts
clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"

# CMake build option
cmake-build:
	@if not exist "build" mkdir build
	@cd build && cmake .. && cmake --build .

cmake-run: cmake-build
	@cd build && cmake --build . --target run

# Help target
help:
	@echo Available targets:
	@echo   all        - Build with GCC/Clang (default)
	@echo   run        - Build and run with GCC/Clang
	@echo   msvc-build - Build with MSVC
	@echo   msvc-run   - Build and run with MSVC
	@echo   cmake-build- Build using CMake
	@echo   cmake-run  - Build and run using CMake
	@echo   clean      - Clean build artifacts
	@echo   help       - Show this help message
