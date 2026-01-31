#pragma once

#include "types.hpp"
#include <tlhelp32.h>
#include <psapi.h>

namespace krash {

class Memory {
public:
    static size_t getAvailable() {
        MEMORYSTATUSEX mem = {sizeof(mem)};
        GlobalMemoryStatusEx(&mem);
        return (size_t)(mem.ullAvailPhys / 1024 / 1024);
    }

    static size_t getTotal() {
        MEMORYSTATUSEX mem = {sizeof(mem)};
        GlobalMemoryStatusEx(&mem);
        return (size_t)(mem.ullTotalPhys / 1024 / 1024);
    }

    static size_t freeWorkingSets() {
        size_t before = getAvailable();

        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snap == INVALID_HANDLE_VALUE) return 0;

        PROCESSENTRY32 pe = {sizeof(pe)};
        if (Process32First(snap, &pe)) {
            do {
                HANDLE hProc = OpenProcess(PROCESS_SET_QUOTA | PROCESS_QUERY_INFORMATION, FALSE, pe.th32ProcessID);
                if (hProc) {
                    SetProcessWorkingSetSize(hProc, (SIZE_T)-1, (SIZE_T)-1);
                    EmptyWorkingSet(hProc);
                    CloseHandle(hProc);
                }
            } while (Process32Next(snap, &pe));
        }
        CloseHandle(snap);

        Sleep(1000);
        size_t after = getAvailable();
        return after > before ? after - before : 0;
    }

    static void flushFileBuffers() {
        HANDLE hToken;
        TOKEN_PRIVILEGES tp;

        if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
            LookupPrivilegeValue(NULL, SE_INCREASE_QUOTA_NAME, &tp.Privileges[0].Luid);
            tp.PrivilegeCount = 1;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
            CloseHandle(hToken);
        }
    }

    static void flushDNS() {
        system("ipconfig /flushdns > nul 2>&1");
    }

    static void flushARP() {
        system("netsh interface ip delete arpcache > nul 2>&1");
    }

    static size_t deepClean() {
        flushFileBuffers();
        flushDNS();
        flushARP();
        return freeWorkingSets();
    }
};

}
