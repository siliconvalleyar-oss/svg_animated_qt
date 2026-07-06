@echo off
echo ========================================
echo SVG Animator - Build Script (Windows)
echo ========================================
echo.

REM Check if Qt5 is in PATH
where qmake >nul 2>&1
if %errorlevel% neq 0 (
    echo [WARNING] qmake not found in PATH.
    echo Please ensure Qt5 is installed and added to PATH.
    echo.
    echo Example paths:
    echo   C:\Qt\5.15.2\mingw81_64\bin
    echo   C:\Qt\5.15.2\msvc2019_64\bin
    echo.
)

REM Create build directory
if not exist "build" mkdir build
cd build

REM Run CMake
echo Running CMake...
cmake ..
if %errorlevel% neq 0 (
    echo [ERROR] CMake failed. Check Qt5 installation.
    pause
    exit /b 1
)

REM Build
echo Building...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build successful!
echo Executable: build\Release\svg-animator.exe
echo ========================================
pause
