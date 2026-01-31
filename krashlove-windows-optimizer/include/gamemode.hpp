#pragma once

#include "types.hpp"
#include <tlhelp32.h>
#include <fstream>

namespace krash {

class GameMode {
private:
    static std::string getStatePath() {
        char path[MAX_PATH];
        GetTempPathA(MAX_PATH, path);
        return std::string(path) + "krashlove_gamemode.state";
    }

    static void saveState(bool state) {
        std::ofstream f(getStatePath());
        if (f.is_open()) {
            f << (state ? 1 : 0);
            f.close();
        }
    }

    static bool loadState() {
        std::ifstream f(getStatePath());
        if (!f.is_open()) return false;
        int state = 0;
        f >> state;
        f.close();
        return state == 1;
    }

public:
    static void enable() {
        SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

        BOOL off = FALSE;
        SystemParametersInfoA(SPI_SETMENUANIMATION, 0, &off, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETCOMBOBOXANIMATION, 0, &off, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETLISTBOXSMOOTHSCROLLING, 0, &off, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETTOOLTIPANIMATION, 0, &off, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETCURSORSHADOW, 0, &off, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETHOTTRACKING, 0, &off, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETSELECTIONFADE, 0, &off, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETMOUSEVANISH, 0, &off, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, SPIF_SENDCHANGE);

        HKEY hKey;
        DWORD val = 0;
        if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
            RegSetValueExA(hKey, "EnableTransparency", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
            RegCloseKey(hKey);
        }
        if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\DWM", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
            RegSetValueExA(hKey, "EnableAeroPeek", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
            RegCloseKey(hKey);
        }

        setHighPerformance();
        saveState(true);
    }

    static void disable() {
        SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);

        BOOL on = TRUE;
        SystemParametersInfoA(SPI_SETMENUANIMATION, 0, &on, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETCOMBOBOXANIMATION, 0, &on, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETLISTBOXSMOOTHSCROLLING, 0, &on, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETTOOLTIPANIMATION, 0, &on, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETCURSORSHADOW, 0, &on, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETHOTTRACKING, 0, &on, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETSELECTIONFADE, 0, &on, SPIF_SENDCHANGE);
        SystemParametersInfoA(SPI_SETDRAGFULLWINDOWS, TRUE, NULL, SPIF_SENDCHANGE);

        HKEY hKey;
        DWORD val = 1;
        if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
            RegSetValueExA(hKey, "EnableTransparency", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
            RegCloseKey(hKey);
        }
        if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\DWM", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
            RegSetValueExA(hKey, "EnableAeroPeek", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
            RegCloseKey(hKey);
        }

        setBalanced();
        saveState(false);
    }

    static void setHighPerformance() {
        system("powercfg /setactive 8c5e7fda-e8bf-4a96-9a85-a6e23a8c635c > nul 2>&1");
    }

    static void setBalanced() {
        system("powercfg /setactive 381b4222-f694-41f0-9685-ff5bb260df2e > nul 2>&1");
    }

    static int killBackgroundApps() {
        const char* targets[] = {
            "OneDrive.exe", "Cortana.exe", "SearchApp.exe", "SearchUI.exe",
            "YourPhone.exe", "PhoneExperienceHost.exe", "GrooveMusic.exe",
            "Microsoft.Photos.exe", "Video.UI.exe", "GameBarPresenceWriter.exe",
            "GameBar.exe", "MicrosoftEdgeUpdate.exe", "UpdateAssistant.exe",
            "MicrosoftEdge.exe", "msedge.exe", "backgroundTaskHost.exe",
            "RuntimeBroker.exe", "SkypeApp.exe", "Skype.exe"
        };

        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snap == INVALID_HANDLE_VALUE) return 0;

        int killed = 0;
        PROCESSENTRY32 pe = {sizeof(pe)};

        if (Process32First(snap, &pe)) {
            do {
                for (const char* target : targets) {
                    if (_stricmp(pe.szExeFile, target) == 0) {
                        HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                        if (hProc) {
                            if (TerminateProcess(hProc, 0)) killed++;
                            CloseHandle(hProc);
                        }
                    }
                }
            } while (Process32Next(snap, &pe));
        }
        CloseHandle(snap);
        return killed;
    }

    static void disableNagle() {
        HKEY hKey;
        const char* path = "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces";
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            DWORD index = 0;
            char subKey[256];
            DWORD subKeyLen;

            while (true) {
                subKeyLen = sizeof(subKey);
                if (RegEnumKeyExA(hKey, index++, subKey, &subKeyLen, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                    break;

                std::string fullPath = std::string(path) + "\\" + subKey;
                HKEY hSubKey;
                if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, fullPath.c_str(), 0, KEY_SET_VALUE, &hSubKey) == ERROR_SUCCESS) {
                    DWORD val = 1;
                    RegSetValueExA(hSubKey, "TcpAckFrequency", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
                    RegSetValueExA(hSubKey, "TCPNoDelay", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
                    RegCloseKey(hSubKey);
                }
            }
            RegCloseKey(hKey);
        }
    }

    static void optimizeForGaming() {
        enable();
        killBackgroundApps();
    }

    static bool isEnabled() { 
        return loadState();
    }
};

}
