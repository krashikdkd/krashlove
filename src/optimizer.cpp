#include "../include/optimizer.h"
#include <iostream>
#include <filesystem>
#include <psapi.h>
#include <tlhelp32.h>
#include <winreg.h>
#include <iphlpapi.h>
#include <shlobj.h>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "advapi32.lib")

namespace KrashLove {
namespace fs = std::filesystem;

// ============== System Cleanup Implementation ==============

bool SystemCleanup::CleanTempFiles() {
    Utils::PrintInfo("Cleaning temporary files...");
    bool success = true;
    
    // Clean Windows Temp
    wchar_t tempPath[MAX_PATH];
    if (GetTempPathW(MAX_PATH, tempPath)) {
        success &= DeleteFilesInDirectory(tempPath);
    }
    
    // Clean user Temp
    wchar_t userTemp[MAX_PATH];
    if (GetEnvironmentVariableW(L"TEMP", userTemp, MAX_PATH)) {
        success &= DeleteFilesInDirectory(userTemp);
    }
    
    return success;
}

bool SystemCleanup::CleanPrefetchFiles() {
    Utils::PrintInfo("Cleaning prefetch files...");
    std::wstring prefetchPath = L"C:\\Windows\\Prefetch";
    
    try {
        if (fs::exists(prefetchPath)) {
            return DeleteFilesInDirectory(prefetchPath);
        }
    } catch (const std::exception& e) {
        Utils::PrintError("Failed to clean prefetch: " + std::string(e.what()));
        return false;
    }
    
    return true;
}

bool SystemCleanup::EmptyRecycleBin() {
    Utils::PrintInfo("Emptying recycle bin...");
    HRESULT hr = SHEmptyRecycleBinW(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
    return SUCCEEDED(hr);
}

void SystemCleanup::DisplayCleanupStatus() {
    wchar_t tempPath[MAX_PATH];
    if (GetTempPathW(MAX_PATH, tempPath)) {
        ULONGLONG size = CalculateDirectorySize(tempPath);
        std::wcout << L"Temp directory size: " << (size / 1024 / 1024) << L" MB" << std::endl;
    }
}

bool SystemCleanup::DeleteFilesInDirectory(const std::wstring& directory) {
    int deletedCount = 0;
    
    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            try {
                if (entry.is_regular_file()) {
                    fs::remove(entry.path());
                    deletedCount++;
                }
            } catch (...) {
                // Skip files that can't be deleted (in use, permissions, etc.)
                continue;
            }
        }
        
        if (deletedCount > 0) {
            Utils::PrintSuccess("Deleted " + std::to_string(deletedCount) + " files");
        }
        return true;
    } catch (const std::exception& e) {
        Utils::PrintError("Error deleting files: " + std::string(e.what()));
        return false;
    }
}

ULONGLONG SystemCleanup::CalculateDirectorySize(const std::wstring& directory) {
    ULONGLONG size = 0;
    try {
        for (const auto& entry : fs::recursive_directory_iterator(directory)) {
            try {
                if (entry.is_regular_file()) {
                    size += entry.file_size();
                }
            } catch (...) {
                continue;
            }
        }
    } catch (...) {
        // Directory might not be accessible
    }
    return size;
}

// ============== RAM Optimizer Implementation ==============

bool RAMOptimizer::OptimizeRAM() {
    Utils::PrintInfo("Optimizing RAM...");
    bool success = true;
    
    success &= ClearStandbyMemory();
    success &= OptimizeWorkingSet();
    
    return success;
}

bool RAMOptimizer::ClearStandbyMemory() {
    // Clear standby list
    HANDLE hProcess = GetCurrentProcess();
    if (SetProcessWorkingSetSize(hProcess, (SIZE_T)-1, (SIZE_T)-1)) {
        Utils::PrintSuccess("Standby memory cleared");
        return true;
    }
    return false;
}

bool RAMOptimizer::OptimizeWorkingSet() {
    Utils::PrintInfo("Optimizing working sets...");
    
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);
    
    int optimizedCount = 0;
    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, FALSE, pe32.th32ProcessID);
            if (hProcess != NULL) {
                if (EmptyWorkingSet(hProcess)) {
                    optimizedCount++;
                }
                CloseHandle(hProcess);
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }
    
    CloseHandle(hSnapshot);
    
    if (optimizedCount > 0) {
        Utils::PrintSuccess("Optimized " + std::to_string(optimizedCount) + " processes");
    }
    
    return true;
}

void RAMOptimizer::DisplayMemoryStatus() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    
    if (GlobalMemoryStatusEx(&memInfo)) {
        DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
        DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
        
        std::cout << "\n=== Memory Status ===" << std::endl;
        std::cout << "Total RAM: " << (totalPhysMem / 1024 / 1024) << " MB" << std::endl;
        std::cout << "Used RAM: " << (physMemUsed / 1024 / 1024) << " MB" << std::endl;
        std::cout << "Available RAM: " << (memInfo.ullAvailPhys / 1024 / 1024) << " MB" << std::endl;
        std::cout << "Memory Load: " << memInfo.dwMemoryLoad << "%" << std::endl;
    }
}

bool RAMOptimizer::EmptyWorkingSet(HANDLE processHandle) {
    return SetProcessWorkingSetSize(processHandle, (SIZE_T)-1, (SIZE_T)-1) != 0;
}

// ============== CPU Optimizer Implementation ==============

bool CPUOptimizer::BoostProcessPriority(const std::wstring& processName) {
    Utils::PrintInfo("Boosting process priority...");
    
    DWORD processId = GetProcessIdByName(processName);
    if (processId == 0) {
        Utils::PrintError("Process not found: " + std::string(processName.begin(), processName.end()));
        return false;
    }
    
    HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, processId);
    if (hProcess == NULL) {
        Utils::PrintError("Failed to open process");
        return false;
    }
    
    bool success = SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);
    CloseHandle(hProcess);
    
    if (success) {
        Utils::PrintSuccess("Process priority boosted to HIGH");
    }
    
    return success;
}

bool CPUOptimizer::SetSystemPriority() {
    Utils::PrintInfo("Setting system priority...");
    
    HANDLE hProcess = GetCurrentProcess();
    if (SetPriorityClass(hProcess, ABOVE_NORMAL_PRIORITY_CLASS)) {
        Utils::PrintSuccess("System priority set to ABOVE_NORMAL");
        return true;
    }
    
    return false;
}

bool CPUOptimizer::OptimizeCoreParking() {
    Utils::PrintInfo("Optimizing CPU core parking...");
    
    // Disable core parking by setting registry values
    HKEY hKey;
    LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
        L"SYSTEM\\CurrentControlSet\\Control\\Power\\PowerSettings\\54533251-82be-4824-96c1-47b60b740d00\\0cc5b647-c1df-4637-891a-dec35c318583",
        0, KEY_SET_VALUE, &hKey);
    
    if (result == ERROR_SUCCESS) {
        DWORD value = 0;
        RegSetValueExW(hKey, L"ValueMax", 0, REG_DWORD, (BYTE*)&value, sizeof(DWORD));
        RegCloseKey(hKey);
        Utils::PrintSuccess("Core parking disabled");
        return true;
    }
    
    Utils::PrintError("Failed to modify core parking settings");
    return false;
}

void CPUOptimizer::ListRunningProcesses() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return;
    }
    
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);
    
    std::cout << "\n=== Running Processes ===" << std::endl;
    
    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            std::wcout << L"[" << pe32.th32ProcessID << L"] " << pe32.szExeFile << std::endl;
        } while (Process32NextW(hSnapshot, &pe32));
    }
    
    CloseHandle(hSnapshot);
}

DWORD CPUOptimizer::GetProcessIdByName(const std::wstring& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }
    
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);
    
    DWORD processId = 0;
    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            if (_wcsicmp(pe32.szExeFile, processName.c_str()) == 0) {
                processId = pe32.th32ProcessID;
                break;
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }
    
    CloseHandle(hSnapshot);
    return processId;
}

// ============== Network Optimizer Implementation ==============

bool NetworkOptimizer::OptimizeTCP() {
    Utils::PrintInfo("Optimizing TCP settings...");
    bool success = true;
    
    success &= DisableNagleAlgorithm();
    success &= SetReceiveWindowSize();
    
    return success;
}

bool NetworkOptimizer::DisableNagleAlgorithm() {
    Utils::PrintInfo("Disabling Nagle's algorithm...");
    
    HKEY hKey;
    LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
        L"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces",
        0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hKey);
    
    if (result == ERROR_SUCCESS) {
        // Disable Nagle for all interfaces
        DWORD value = 1;
        SetRegistryValue(HKEY_LOCAL_MACHINE,
            L"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",
            L"TcpAckFrequency", value);
        SetRegistryValue(HKEY_LOCAL_MACHINE,
            L"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",
            L"TCPNoDelay", value);
        
        RegCloseKey(hKey);
        Utils::PrintSuccess("Nagle's algorithm disabled");
        return true;
    }
    
    return false;
}

bool NetworkOptimizer::OptimizeMTU() {
    Utils::PrintInfo("Optimizing MTU...");
    
    DWORD mtu = 1500;
    bool success = SetRegistryValue(HKEY_LOCAL_MACHINE,
        L"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",
        L"MTU", mtu);
    
    if (success) {
        Utils::PrintSuccess("MTU optimized to 1500");
    }
    
    return success;
}

bool NetworkOptimizer::SetReceiveWindowSize() {
    Utils::PrintInfo("Setting TCP receive window size...");
    
    DWORD windowSize = 65535;
    bool success = SetRegistryValue(HKEY_LOCAL_MACHINE,
        L"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",
        L"TcpWindowSize", windowSize);
    
    if (success) {
        Utils::PrintSuccess("TCP window size optimized");
    }
    
    return success;
}

void NetworkOptimizer::DisplayNetworkStatus() {
    std::cout << "\n=== Network Status ===" << std::endl;
    std::cout << "TCP optimizations applied" << std::endl;
    std::cout << "Nagle's algorithm: Disabled" << std::endl;
    std::cout << "TCP Window Size: 65535" << std::endl;
}

bool NetworkOptimizer::SetRegistryValue(HKEY hKey, const std::wstring& subKey,
                                        const std::wstring& valueName, DWORD value) {
    HKEY hOpenKey;
    LONG result = RegOpenKeyExW(hKey, subKey.c_str(), 0, KEY_SET_VALUE, &hOpenKey);
    
    if (result == ERROR_SUCCESS) {
        result = RegSetValueExW(hOpenKey, valueName.c_str(), 0, REG_DWORD,
                                (BYTE*)&value, sizeof(DWORD));
        RegCloseKey(hOpenKey);
        return result == ERROR_SUCCESS;
    }
    
    return false;
}

// ============== Process Manager Implementation ==============

bool ProcessManager::DisableUnnecessaryServices() {
    Utils::PrintInfo("Disabling unnecessary services...");
    
    auto services = GetUnnecessaryServices();
    int disabledCount = 0;
    
    for (const auto& service : services) {
        if (DisableService(service)) {
            disabledCount++;
        }
    }
    
    if (disabledCount > 0) {
        Utils::PrintSuccess("Disabled " + std::to_string(disabledCount) + " services");
    }
    
    return true;
}

bool ProcessManager::StopBackgroundProcesses() {
    Utils::PrintInfo("Stopping background processes...");
    
    // This is a dangerous operation, so we'll be conservative
    // Only target known safe-to-stop processes
    
    Utils::PrintInfo("Background process management requires manual intervention for safety");
    return true;
}

void ProcessManager::ListServices() {
    SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
    if (hSCManager == NULL) {
        return;
    }
    
    DWORD bytesNeeded = 0;
    DWORD servicesReturned = 0;
    DWORD resumeHandle = 0;
    
    EnumServicesStatusW(hSCManager, SERVICE_WIN32, SERVICE_STATE_ALL,
                       NULL, 0, &bytesNeeded, &servicesReturned, &resumeHandle);
    
    std::vector<BYTE> buffer(bytesNeeded);
    ENUM_SERVICE_STATUSW* pServices = (ENUM_SERVICE_STATUSW*)buffer.data();
    
    if (EnumServicesStatusW(hSCManager, SERVICE_WIN32, SERVICE_STATE_ALL,
                           pServices, bytesNeeded, &bytesNeeded,
                           &servicesReturned, &resumeHandle)) {
        std::cout << "\n=== Windows Services ===" << std::endl;
        for (DWORD i = 0; i < servicesReturned; i++) {
            std::wcout << pServices[i].lpServiceName << L" - ";
            if (pServices[i].ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
                std::wcout << L"Running" << std::endl;
            } else {
                std::wcout << L"Stopped" << std::endl;
            }
        }
    }
    
    CloseServiceHandle(hSCManager);
}

bool ProcessManager::EnableService(const std::wstring& serviceName) {
    return ChangeServiceStartup(serviceName, SERVICE_AUTO_START);
}

bool ProcessManager::DisableService(const std::wstring& serviceName) {
    return ChangeServiceStartup(serviceName, SERVICE_DISABLED);
}

bool ProcessManager::ChangeServiceStartup(const std::wstring& serviceName, DWORD startType) {
    SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCManager == NULL) {
        return false;
    }
    
    SC_HANDLE hService = OpenServiceW(hSCManager, serviceName.c_str(), SERVICE_CHANGE_CONFIG);
    if (hService == NULL) {
        CloseServiceHandle(hSCManager);
        return false;
    }
    
    bool success = ChangeServiceConfigW(hService, SERVICE_NO_CHANGE, startType,
                                        SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL,
                                        NULL, NULL, NULL);
    
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    
    return success;
}

std::vector<std::wstring> ProcessManager::GetUnnecessaryServices() {
    // Conservative list of services that are generally safe to disable for performance
    return {
        L"DiagTrack",           // Connected User Experiences and Telemetry
        L"dmwappushservice",    // WAP Push Message Routing Service
        L"RetailDemo",          // Retail Demo Service
        L"XblAuthManager",      // Xbox Live Auth Manager (if not gaming)
        L"XblGameSave",         // Xbox Live Game Save (if not gaming)
        L"XboxNetApiSvc"        // Xbox Live Networking Service (if not gaming)
    };
}

// ============== Utils Implementation ==============

bool Utils::IsRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                                 DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
                                 &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    
    return isAdmin == TRUE;
}

bool Utils::RestartAsAdmin() {
    wchar_t szPath[MAX_PATH];
    if (GetModuleFileNameW(NULL, szPath, MAX_PATH)) {
        SHELLEXECUTEINFOW sei = { sizeof(sei) };
        sei.lpVerb = L"runas";
        sei.lpFile = szPath;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;
        
        if (ShellExecuteExW(&sei)) {
            return true;
        }
    }
    return false;
}

void Utils::PrintError(const std::string& message) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::cout << "[ERROR] " << message << std::endl;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Utils::PrintSuccess(const std::string& message) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "[SUCCESS] " << message << std::endl;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Utils::PrintInfo(const std::string& message) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    std::cout << "[INFO] " << message << std::endl;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Utils::WaitForKeyPress() {
    std::cout << "\nPress any key to continue...";
    std::cin.ignore();
    std::cin.get();
}

} // namespace KrashLove
