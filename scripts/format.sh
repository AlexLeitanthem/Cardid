#!/bin/bash
# Format code using clang-format

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Formatting code with clang-format...${NC}"

# Check if clang-format is available
if ! command -v clang-format &> /dev/null; then
    echo -e "${RED}Error: clang-format not found. Please install clang-format.${NC}"
    exit 1
fi

# Find all C/C++ files
FILES=$(find . -name "*.c" -o -name "*.h" | grep -v build | grep -v .git)

if [ -z "$FILES" ]; then
    echo -e "${YELLOW}No C/C++ files found to format.${NC}"
    exit 0
fi

# Format files
for file in $FILES; do
    echo "Formatting $file"
    clang-format -i "$file"
done

echo -e "${GREEN}Code formatting completed!${NC}"
