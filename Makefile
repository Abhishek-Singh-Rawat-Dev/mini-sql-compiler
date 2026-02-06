# ============================================================================
#                 MINI SQL COMPILER - BUILD CONFIGURATION
# ============================================================================
# 
# Makefile for building the Mini SQL Compiler project
# Team Member 4 Responsibility: Build System and Testing
#
# Usage:
#   make          - Build the project
#   make run      - Build and run
#   make demo     - Build and run demo mode
#   make clean    - Remove build artifacts
#   make test     - Run test queries
# ============================================================================

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Output executable
TARGET = sql_compiler

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Default target
all: $(BUILD_DIR) $(TARGET)
	@echo "Build complete! Run with ./$(TARGET)"

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Successfully built $(TARGET)"

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Run the compiler
run: $(TARGET)
	@./$(TARGET)

# Run demo mode
demo: $(TARGET)
	@./$(TARGET) --demo

# Run test queries  
test: $(TARGET)
	@echo "=== Testing Valid Query 1 ==="
	@echo "SELECT * FROM employees;" | ./$(TARGET)
	@echo ""
	@echo "=== Testing Valid Query 2 ==="
	@echo "SELECT name, age FROM users WHERE age > 25;" | ./$(TARGET)
	@echo ""
	@echo "=== Testing Invalid Query (Syntax Error) ==="
	@echo "SELECT * employees;" | ./$(TARGET)
	@echo ""
	@echo "=== Testing Invalid Query (Semantic Error) ==="
	@echo "SELECT * FROM nonexistent;" | ./$(TARGET)

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Clean complete"

# Help target
help:
	@echo "Mini SQL Compiler - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  make        - Build the project"
	@echo "  make run    - Build and run interactive mode"
	@echo "  make demo   - Build and run demo mode"
	@echo "  make test   - Run automated tests"
	@echo "  make clean  - Remove build artifacts"
	@echo "  make help   - Show this help message"

# Phony targets
.PHONY: all run demo test clean help
