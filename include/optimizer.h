#pragma once

#include <string>
#include <vector>
#include <windows.h>

namespace KrashLove {

// Network optimization constants
constexpr DWORD OPTIMAL_MTU = 1500;
constexpr DWORD OPTIMAL_TCP_WINDOW_SIZE = 65535;

// System Cleanup Module
class SystemCleanup {
public:
    static bool CleanTempFiles();
    static bool CleanPrefetchFiles();
    static bool EmptyRecycleBin();
    static void DisplayCleanupStatus();
private:
    static bool DeleteFilesInDirectory(const std::wstring& directory);
    static ULONGLONG CalculateDirectorySize(const std::wstring& directory);
};

// RAM Optimization Module
class RAMOptimizer {
public:
    static bool OptimizeRAM();
    static bool ClearStandbyMemory();
    static bool OptimizeWorkingSet();
    static void DisplayMemoryStatus();
private:
    static bool EmptyWorkingSet(HANDLE processHandle);
};

// CPU Priority Boosting Module
class CPUOptimizer {
public:
    static bool BoostProcessPriority(const std::wstring& processName);
    static bool SetSystemPriority();
    static bool OptimizeCoreParking();
    static void ListRunningProcesses();
private:
    static DWORD GetProcessIdByName(const std::wstring& processName);
};

// Network Optimization Module
class NetworkOptimizer {
public:
    static bool OptimizeTCP();
    static bool DisableNagleAlgorithm();
    static bool OptimizeMTU();
    static bool SetReceiveWindowSize();
    static void DisplayNetworkStatus();
private:
    static bool SetRegistryValue(HKEY hKey, const std::wstring& subKey, 
                                  const std::wstring& valueName, DWORD value);
};

// Background Process Management Module
class ProcessManager {
public:
    static bool DisableUnnecessaryServices();
    static bool StopBackgroundProcesses();
    static void ListServices();
    static bool EnableService(const std::wstring& serviceName);
    static bool DisableService(const std::wstring& serviceName);
private:
    static bool ChangeServiceStartup(const std::wstring& serviceName, DWORD startType);
    static std::vector<std::wstring> GetUnnecessaryServices();
};

// Utility Functions
class Utils {
public:
    static bool IsRunningAsAdmin();
    static bool RestartAsAdmin();
    static void PrintError(const std::string& message);
    static void PrintSuccess(const std::string& message);
    static void PrintInfo(const std::string& message);
    static void WaitForKeyPress();
};

} // namespace KrashLove
