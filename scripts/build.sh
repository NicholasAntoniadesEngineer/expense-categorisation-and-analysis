#!/bin/bash

# Exit on any error
set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"
CODE_DIR="$PROJECT_ROOT/code"
BUILD_DIR="$CODE_DIR/build"

# Parse command line arguments
CLEAN_BUILD=0
BUILD_TYPE="Release"

# Process command line arguments
while [[ $# -gt 0 ]]; do
    key="$1"
    case $key in
        --clean)
            CLEAN_BUILD=1
            shift
            ;;
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

echo "Build Configuration:"
echo "  Build Type: $BUILD_TYPE"
echo "  Clean Build: $([[ $CLEAN_BUILD == 1 ]] && echo "Yes" || echo "No")"
echo ""

if [[ $CLEAN_BUILD == 1 ]]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

cd "$BUILD_DIR"

echo "Configuring with CMake..."
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..

echo "Building..."
cmake --build . -j$(sysctl -n hw.ncpu)

echo "Build complete!"
echo "Executable location: $BUILD_DIR/bin/finance_categorisation" 