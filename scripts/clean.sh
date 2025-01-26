#!/bin/bash

set -e

# Core paths
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/code/build"

echo "Cleaning project..."
echo "  - Build directory: $BUILD_DIR"
echo "  - Generated files in files_categorised/"

# Remove build directory
rm -rf "$BUILD_DIR"

CATEGORISED_DIR="$PROJECT_ROOT/files_categorised"
if [ -d "$CATEGORISED_DIR" ]; then
    echo "Removing generated files in files_categorised..."
    rm -f "$CATEGORISED_DIR"/*.csv
fi

echo "Clean complete!" 