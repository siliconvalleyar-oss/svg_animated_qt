@echo off
echo Starting SVG Animator...
echo.

if exist "build\Release\svg-animator.exe" (
    start "" "build\Release\svg-animator.exe"
) else if exist "build\svg-animator.exe" (
    start "" "build\svg-animator.exe"
) else (
    echo [ERROR] Executable not found.
    echo Please build the project first using build.bat
    pause
)
