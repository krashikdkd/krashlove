#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <functional>

namespace krash {

enum class Color : WORD {
    Black = 0, Blue = 1, Green = 2, Cyan = 3, Red = 4, Magenta = 5, Yellow = 6, White = 7,
    Gray = 8, BrightBlue = 9, BrightGreen = 10, BrightCyan = 11,
    BrightRed = 12, BrightMagenta = 13, BrightYellow = 14, BrightWhite = 15
};

struct SystemStats {
    std::string cpuName;
    std::string gpuName;
    std::string osName;
    std::string biosVersion;
    std::string motherboard;
    size_t totalRam;
    size_t availableRam;
    int ramUsagePercent;
    size_t diskFree;
    size_t diskTotal;
    int processCount;
    int threadCount;
    int handleCount;
    double cpuUsage;
    int cpuCores;
    int cpuThreads;
    std::string ipAddress;
    size_t networkSent;
    size_t networkRecv;
};

struct CleanResult {
    int filesDeleted;
    size_t bytesFreed;
};

struct ProcessInfo {
    DWORD pid;
    std::string name;
    size_t memory;
    int priority;
};

}
