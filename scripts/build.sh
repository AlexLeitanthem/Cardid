#!/bin/bash
# Build script for CardID

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Default values
BUILD_TYPE="Release"
BUILD_TESTS="ON"
BUILD_CLI="ON"
CLEAN_BUILD=false
RUN_TESTS=false
INSTALL=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -r|--release)
            BUILD_TYPE="Release"
            shift
            ;;
        --no-tests)
            BUILD_TESTS="OFF"
            shift
            ;;
        --no-cli)
            BUILD_CLI="OFF"
            shift
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -t|--test)
            RUN_TESTS=true
            shift
            ;;
        -i|--install)
            INSTALL=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  -d, --debug      Build in Debug mode"
            echo "  -r, --release    Build in Release mode (default)"
            echo "  --no-tests       Disable test building"
            echo "  --no-cli         Disable CLI building"
            echo "  -c, --clean      Clean build directory"
            echo "  -t, --test       Run tests after building"
            echo "  -i, --install    Install after building"
            echo "  -h, --help       Show this help message"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            exit 1
            ;;
    esac
done

echo -e "${BLUE}CardID Build Script${NC}"
echo "=================="
echo "Build Type: $BUILD_TYPE"
echo "Build Tests: $BUILD_TESTS"
echo "Build CLI: $BUILD_CLI"
echo "Clean Build: $CLEAN_BUILD"
echo "Run Tests: $RUN_TESTS"
echo "Install: $INSTALL"
echo ""

# Clean build directory if requested
if [ "$CLEAN_BUILD" = true ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf build
fi

# Create build directory
mkdir -p build

# Configure with CMake
echo -e "${YELLOW}Configuring with CMake...${NC}"
cmake -S . -B build \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DBUILD_TESTS="$BUILD_TESTS" \
    -DBUILD_CLI="$BUILD_CLI"

# Build
echo -e "${YELLOW}Building...${NC}"
cmake --build build --parallel

# Run tests if requested
if [ "$RUN_TESTS" = true ] && [ "$BUILD_TESTS" = "ON" ]; then
    echo -e "${YELLOW}Running tests...${NC}"
    cmake --build build --target test
fi

# Install if requested
if [ "$INSTALL" = true ]; then
    echo -e "${YELLOW}Installing...${NC}"
    sudo cmake --install build
fi

echo -e "${GREEN}Build completed successfully!${NC}"

# Show build artifacts
echo -e "${BLUE}Build artifacts:${NC}"
if [ "$BUILD_CLI" = "ON" ]; then
    if [ -f "build/cardid" ]; then
        echo "  CLI: build/cardid"
    elif [ -f "build/cardid.exe" ]; then
        echo "  CLI: build/cardid.exe"
    fi
fi

if [ "$BUILD_TESTS" = "ON" ]; then
    if [ -f "build/tests/test_cardid" ]; then
        echo "  Tests: build/tests/test_cardid"
    elif [ -f "build/tests/test_cardid.exe" ]; then
        echo "  Tests: build/tests/test_cardid.exe"
    fi
fi

echo "  Library: build/libcardid.a (or equivalent)"
