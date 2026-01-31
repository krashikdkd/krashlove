# KrashLove - Windows Performance Optimizer

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Platform](https://img.shields.io/badge/platform-Windows-blue.svg)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

A hardcore Windows optimization tool written in C++17. Console application focused on maximum FPS, low latency, and system stability through advanced system-level optimizations.

## Features

### 🧹 System Cleanup
- **Temp Files Cleanup**: Removes temporary files from Windows and user temp directories
- **Prefetch Cleanup**: Cleans Windows prefetch directory
- **Recycle Bin**: Empties recycle bin
- **Real-time Status**: Displays current directory sizes before cleanup

### 💾 RAM Optimization
- **Standby Memory Clear**: Frees up standby memory
- **Working Set Optimization**: Optimizes working sets for all running processes
- **Memory Status Display**: Shows real-time RAM usage statistics
- **Automatic Memory Freed**: Reduces memory footprint across system

### ⚡ CPU Priority Boosting
- **Process Priority Boost**: Boost specific processes to HIGH priority
- **System Priority**: Set system-wide priority optimization
- **CPU Core Parking**: Disable core parking for maximum performance
- **Process Listing**: View all running processes with PID

### 🌐 Network (TCP) Optimization
- **Nagle's Algorithm**: Disable Nagle's algorithm for lower latency
- **TCP Window Size**: Optimize TCP receive window size (65535 bytes)
- **MTU Optimization**: Set optimal MTU settings (1500)
- **Network Status**: Display current network optimization state

### 🔧 Background Process Management
- **Disable Unnecessary Services**: Automatically disable telemetry and Xbox services
- **Service Control**: Enable/disable specific Windows services
- **Service Listing**: View all Windows services and their status
- **Safe Operations**: Conservative approach to avoid system instability

## Requirements

- **Operating System**: Windows 10/11 (64-bit)
- **Compiler**: MSVC, GCC (MinGW-w64), or Clang with C++17 support
- **CMake**: Version 3.15 or higher
- **Administrator Rights**: Required for system-level operations

## Building

### Using CMake (Recommended)

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
cmake --build . --config Release

# The executable will be in build/bin/KrashLove.exe
```

### Using MSVC (Visual Studio)

```bash
# Using Developer Command Prompt
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

### Using MinGW

```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build . --config Release
```

## Usage

### Running the Application

**Important**: The application must be run with administrator privileges!

```bash
# Run directly (will prompt for admin rights)
KrashLove.exe

# Or right-click and "Run as administrator"
```

### Menu Options

```
1. System Cleanup - Clean temporary files, prefetch, and recycle bin
2. RAM Optimization - Free memory and optimize working sets
3. CPU Priority Boost - Boost process priorities and optimize CPU
4. Network Optimization - Optimize TCP settings for low latency
5. Background Process Management - Control Windows services
6. Run All Optimizations - Apply all optimizations at once
7. Display System Status - View current system statistics
0. Exit - Exit the application
```

### Quick Start Guide

1. **Run as Administrator**: The application will automatically request admin privileges
2. **Check System Status**: Select option 7 to view current system state
3. **Run All Optimizations**: Select option 6 for a complete optimization
4. **Restart System**: Restart your computer for all changes to take effect

## Safety and Warnings

⚠️ **Important Warnings**:
- This tool modifies system settings and requires administrator privileges
- Some optimizations require a system restart to take effect
- Registry modifications are permanent until manually reverted
- Service changes can affect system functionality
- Always backup your system before running aggressive optimizations

✅ **Safe Practices**:
- All operations are designed to be safe and reversible
- Conservative service disable list (only telemetry/Xbox services)
- No critical system services are modified
- File deletions only target temp directories
- Memory optimizations are non-destructive

## Technical Details

### Architecture
- **Language**: C++17
- **Platform**: Windows API (Win32)
- **Libraries Used**: 
  - `psapi.lib` - Process and memory management
  - `iphlpapi.lib` - Network configuration
  - `advapi32.lib` - Registry and service management
  - `shell32.lib` - Shell operations

### Modules
1. **SystemCleanup**: Handles file cleanup operations
2. **RAMOptimizer**: Memory optimization and working set management
3. **CPUOptimizer**: Process priority and CPU parking control
4. **NetworkOptimizer**: TCP/IP stack optimization
5. **ProcessManager**: Windows service management
6. **Utils**: Helper functions and console output

### Key APIs Used
- `SetProcessWorkingSetSize()` - Memory optimization
- `SetPriorityClass()` - Process priority control
- `RegSetValueEx()` - Registry modifications
- `ChangeServiceConfig()` - Service management
- `SHEmptyRecycleBin()` - Recycle bin operations

## Performance Impact

Expected improvements after optimization:
- **RAM Usage**: 10-30% reduction in memory footprint
- **CPU Latency**: Lower process scheduling delays
- **Network Latency**: Reduced TCP overhead and faster response times
- **Disk Space**: Recovery of 100MB-2GB from temp files
- **Background Load**: Reduced CPU usage from disabled services

## Troubleshooting

### "This program requires administrator privileges"
- Right-click the executable and select "Run as administrator"
- Or allow the UAC prompt when the application requests elevation

### "Failed to modify core parking settings"
- Ensure you're running with admin rights
- Some Windows editions may restrict registry access
- Antivirus software may block registry modifications

### Changes not taking effect
- Most optimizations require a system restart
- Check Windows Event Viewer for errors
- Verify services are actually disabled in services.msc

## Contributing

Contributions are welcome! Please follow these guidelines:
1. Maintain C++17 standard compliance
2. Follow existing code style and structure
3. Test on Windows 10 and Windows 11
4. Document all new features
5. Ensure all operations are safe and reversible

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Disclaimer

This software is provided "as is" without warranty of any kind. Use at your own risk. The authors are not responsible for any damage or data loss resulting from the use of this software. Always backup your system before running optimization tools.

## Acknowledgments

- Built with modern C++17 for performance and safety
- Inspired by the need for lightweight, command-line system optimization
- Designed for gamers and performance enthusiasts

---

**Made with ⚡ by KrashLove - For those who demand maximum performance**
