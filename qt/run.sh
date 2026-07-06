#!/bin/bash
echo "Starting SVG Animator..."
echo

if [ -f "./build/svg-animator" ]; then
    ./build/svg-animator
else
    echo "[ERROR] Executable not found."
    echo "Please build the project first using ./build.sh"
    exit 1
fi
