#!/bin/bash
echo "========================================"
echo "SVG Animator - Build Script (Mac/Linux)"
echo "========================================"
echo

# Check if Qt5 is available
if ! command -v qmake &> /dev/null; then
    echo "[WARNING] qmake not found in PATH."
    echo "Please ensure Qt5 is installed."
    echo
    echo "Install commands:"
    echo "  Mac:   brew install qt@5"
    echo "  Linux: sudo apt install qt5-default qtbase5-dev libqt5svg5-dev"
    echo
fi

# Check for required tools
if ! command -v cmake &> /dev/null; then
    echo "[ERROR] CMake not found. Please install CMake."
    exit 1
fi

if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    echo "[ERROR] No C++ compiler found. Please install g++ or clang++."
    exit 1
fi

# Create build directory
mkdir -p build
cd build

# Run CMake
echo "Running CMake..."
cmake ..
if [ $? -ne 0 ]; then
    echo "[ERROR] CMake failed. Check Qt5 installation."
    exit 1
fi

# Build
echo "Building..."
cmake --build . -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
if [ $? -ne 0 ]; then
    echo "[ERROR] Build failed."
    exit 1
fi

echo
echo "========================================"
echo "Build successful!"
echo "Executable: ./build/svg-animator"
echo "========================================"
