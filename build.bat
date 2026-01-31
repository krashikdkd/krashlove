@echo off
REM Build script for KrashLove Windows Optimizer
REM Requires CMake and MSVC/MinGW

echo ================================================
echo    KrashLove Windows Optimizer Build Script
echo ================================================
echo.

REM Check if CMake is installed
cmake --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake is not installed or not in PATH
    echo Please install CMake from https://cmake.org/download/
    pause
    exit /b 1
)

echo CMake found!
echo.

REM Create build directory
if not exist build mkdir build
cd build

echo Configuring project with CMake...
cmake .. -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo ERROR: CMake configuration failed
    cd ..
    pause
    exit /b 1
)

echo.
echo Building project...
cmake --build . --config Release
if errorlevel 1 (
    echo ERROR: Build failed
    cd ..
    pause
    exit /b 1
)

echo.
echo ================================================
echo Build completed successfully!
echo ================================================
echo Executable location: build\bin\Release\KrashLove.exe
echo.
echo Run the executable as Administrator for full functionality.
echo.

cd ..
pause
