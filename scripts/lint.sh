#!/bin/bash
# Lint code using cppcheck and other tools

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Running code quality checks...${NC}"

# Check if cppcheck is available
if ! command -v cppcheck &> /dev/null; then
    echo -e "${YELLOW}Warning: cppcheck not found. Skipping static analysis.${NC}"
    echo "Install with: sudo apt-get install cppcheck (Ubuntu/Debian)"
    echo "Or: brew install cppcheck (macOS)"
else
    echo -e "${YELLOW}Running cppcheck static analysis...${NC}"
    cppcheck --enable=all --error-exitcode=1 --inconclusive --std=c11 \
        --suppress=missingIncludeSystem \
        --suppress=unusedFunction \
        src/ include/
    echo -e "${GREEN}Static analysis passed!${NC}"
fi

# Check if clang-format is available
if ! command -v clang-format &> /dev/null; then
    echo -e "${YELLOW}Warning: clang-format not found. Skipping format check.${NC}"
else
    echo -e "${YELLOW}Checking code formatting...${NC}"
    FILES=$(find . -name "*.c" -o -name "*.h" | grep -v build | grep -v .git)
    
    for file in $FILES; do
        if ! clang-format --dry-run --Werror "$file" > /dev/null 2>&1; then
            echo -e "${RED}Error: $file is not properly formatted${NC}"
            echo "Run: clang-format -i $file"
            exit 1
        fi
    done
    echo -e "${GREEN}Code formatting check passed!${NC}"
fi

echo -e "${GREEN}All code quality checks passed!${NC}"
