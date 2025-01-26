#!/bin/bash
set -e

# Core paths
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/code/build"

# Determine executable path
if [[ "$OSTYPE" == "darwin"* ]]; then
    EXECUTABLE="$BUILD_DIR/bin/FinanceManager.app/Contents/MacOS/FinanceManager"
else
    EXECUTABLE="$BUILD_DIR/bin/FinanceManager"
fi

# Run the application
"$EXECUTABLE" 