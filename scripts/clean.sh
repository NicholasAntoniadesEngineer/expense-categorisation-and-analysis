#!/bin/bash

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$( cd "$SCRIPT_DIR/.." && pwd )"
CODE_DIR="$PROJECT_ROOT/code"
BUILD_DIR="$CODE_DIR/build"

echo "Cleaning project..."
echo "  - Build directory: $BUILD_DIR"
echo "  - Generated files in files_categorised/"

if [ -d "$BUILD_DIR" ]; then
    echo "Removing build directory..."
    rm -rf "$BUILD_DIR"
fi

CATEGORISED_DIR="$PROJECT_ROOT/files_categorised"
if [ -d "$CATEGORISED_DIR" ]; then
    echo "Removing generated files in files_categorised..."
    rm -f "$CATEGORISED_DIR"/*.csv
fi

echo "Clean complete!" 