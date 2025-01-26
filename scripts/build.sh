#!/bin/bash

# Exit on any error
set -e

# Core paths
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/code/build"

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Generate build files
cmake ..

# Get number of CPU cores for parallel build
NUM_CORES=$(sysctl -n hw.ncpu)
echo "Building with $NUM_CORES parallel jobs..."

# Build using all available cores
make -j"$NUM_CORES"

echo "Build complete!"
echo "Executable location: $BUILD_DIR/bin/FinanceManager" 