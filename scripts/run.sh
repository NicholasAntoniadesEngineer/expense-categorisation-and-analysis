#!/bin/bash

# Exit on any error
set -e

# Script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Project root directory (parent of script directory)
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"
# Code directory
CODE_DIR="$PROJECT_ROOT/code"
# Build directory
BUILD_DIR="$CODE_DIR/build"

# Determine the executable path based on OS
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    EXECUTABLE="$BUILD_DIR/bin/finance_categorisation.app/Contents/MacOS/finance_categorisation"
else
    # Linux/Other
    EXECUTABLE="$BUILD_DIR/bin/finance_categorisation"
fi

# Parse command line arguments
BUILD_FIRST=0

# Process command line arguments
while [[ $# -gt 0 ]]; do
    key="$1"
    case $key in
        --build)
            BUILD_FIRST=1
            shift
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 [--build]"
            echo "Options:"
            echo "  --build    Build the project before running"
            exit 1
            ;;
    esac
done

# Check if executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "Executable not found. Building project first..."
    BUILD_FIRST=1
fi

# Build if requested or needed
if [ $BUILD_FIRST -eq 1 ]; then
    echo "Building project..."
    "$SCRIPT_DIR/build.sh"
fi

# Run the application
echo "Running Finance Categorisation..."
"$EXECUTABLE" 