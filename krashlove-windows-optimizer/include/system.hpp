#pragma once

#include "types.hpp"
#include <psapi.h>
#include <tlhelp32.h>
#include <intrin.h>
#include <iphlpapi.h>
#include <winsock2.h>
#include <pdh.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "pdh.lib")

namespace krash {

class System {
public:
    static std::string getCPU() {
        HKEY hKey;
        char buf[256] = {0};
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            DWORD size = sizeof(buf);
            RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)buf, &size);
            RegCloseKey(hKey);
        }
        return buf[0] ? buf : "Unknown";
    }

    static std::string getGPU() {
        DISPLAY_DEVICEA dd;
        dd.cb = sizeof(dd);
        return EnumDisplayDevicesA(NULL, 0, &dd, 0) ? dd.DeviceString : "Unknown";
    }

    static std::string getOS() {
        HKEY hKey;
        char buf[256] = {0};
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            DWORD size = sizeof(buf);
            RegQueryValueExA(hKey, "ProductName", NULL, NULL, (LPBYTE)buf, &size);
            RegCloseKey(hKey);
        }
        return buf[0] ? buf : "Windows";
    }

    static std::string getBIOS() {
        HKEY hKey;
        char buf[256] = {0};
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            DWORD size = sizeof(buf);
            RegQueryValueExA(hKey, "BIOSVersion", NULL, NULL, (LPBYTE)buf, &size);
            RegCloseKey(hKey);
        }
        return buf[0] ? buf : "Unknown";
    }

    static std::string getMotherboard() {
        HKEY hKey;
        char manufacturer[128] = {0}, product[128] = {0};
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            DWORD size = sizeof(manufacturer);
            RegQueryValueExA(hKey, "BaseBoardManufacturer", NULL, NULL, (LPBYTE)manufacturer, &size);
            size = sizeof(product);
            RegQueryValueExA(hKey, "BaseBoardProduct", NULL, NULL, (LPBYTE)product, &size);
            RegCloseKey(hKey);
        }
        return std::string(manufacturer) + " " + product;
    }

    static size_t getTotalRAM() {
        MEMORYSTATUSEX mem = {sizeof(mem)};
        GlobalMemoryStatusEx(&mem);
        return (size_t)(mem.ullTotalPhys / 1024 / 1024);
    }

    static size_t getAvailableRAM() {
        MEMORYSTATUSEX mem = {sizeof(mem)};
        GlobalMemoryStatusEx(&mem);
        return (size_t)(mem.ullAvailPhys / 1024 / 1024);
    }

    static int getRAMUsage() {
        MEMORYSTATUSEX mem = {sizeof(mem)};
        GlobalMemoryStatusEx(&mem);
        return (int)mem.dwMemoryLoad;
    }

    static size_t getDiskFree(const char* drive = "C:\\") {
        ULARGE_INTEGER freeBytes, totalBytes, totalFree;
        if (GetDiskFreeSpaceExA(drive, &freeBytes, &totalBytes, &totalFree))
            return (size_t)(freeBytes.QuadPart / 1024 / 1024 / 1024);
        return 0;
    }

    static size_t getDiskTotal(const char* drive = "C:\\") {
        ULARGE_INTEGER freeBytes, totalBytes, totalFree;
        if (GetDiskFreeSpaceExA(drive, &freeBytes, &totalBytes, &totalFree))
            return (size_t)(totalBytes.QuadPart / 1024 / 1024 / 1024);
        return 0;
    }

    static int getProcessCount() {
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        int count = 0;
        if (snap != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 pe = {sizeof(pe)};
            if (Process32First(snap, &pe)) { do { count++; } while (Process32Next(snap, &pe)); }
            CloseHandle(snap);
        }
        return count;
    }

    static int getCPUCores() {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        return si.dwNumberOfProcessors;
    }

    static std::vector<ProcessInfo> getProcesses() {
        std::vector<ProcessInfo> procs;
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snap != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 pe = {sizeof(pe)};
            if (Process32First(snap, &pe)) {
                do {
                    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
                    if (hProc) {
                        PROCESS_MEMORY_COUNTERS pmc;
                        ProcessInfo info;
                        info.pid = pe.th32ProcessID;
                        info.name = pe.szExeFile;
                        if (GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc)))
                            info.memory = pmc.WorkingSetSize / 1024 / 1024;
                        else
                            info.memory = 0;
                        info.priority = GetPriorityClass(hProc);
                        procs.push_back(info);
                        CloseHandle(hProc);
                    }
                } while (Process32Next(snap, &pe));
            }
            CloseHandle(snap);
        }
        return procs;
    }

    static SystemStats getStats() {
        SystemStats s;
        s.cpuName = getCPU();
        s.gpuName = getGPU();
        s.osName = getOS();
        s.biosVersion = getBIOS();
        s.motherboard = getMotherboard();
        s.totalRam = getTotalRAM();
        s.availableRam = getAvailableRAM();
        s.ramUsagePercent = getRAMUsage();
        s.diskFree = getDiskFree();
        s.diskTotal = getDiskTotal();
        s.processCount = getProcessCount();
        s.cpuCores = getCPUCores();
        return s;
    }
};

}
