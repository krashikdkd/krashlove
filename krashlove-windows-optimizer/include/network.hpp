#pragma once

#include "types.hpp"

namespace krash {

class Network {
public:
    static void flushDNS() {
        system("ipconfig /flushdns > nul 2>&1");
    }

    static void resetWinsock() {
        system("netsh winsock reset > nul 2>&1");
    }

    static void resetTCP() {
        system("netsh int ip reset > nul 2>&1");
    }

    static void flushARP() {
        system("netsh interface ip delete arpcache > nul 2>&1");
    }

    static void optimizeTCP() {
        HKEY hKey;
        const char* path = "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters";
        
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
            DWORD val;
            
            val = 1;
            RegSetValueExA(hKey, "TcpNoDelay", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
            
            val = 1;
            RegSetValueExA(hKey, "TcpAckFrequency", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
            
            val = 65535;
            RegSetValueExA(hKey, "TcpWindowSize", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
            
            val = 65535;
            RegSetValueExA(hKey, "GlobalMaxTcpWindowSize", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
            
            val = 0;
            RegSetValueExA(hKey, "SackOpts", 0, REG_DWORD, (BYTE*)&val, sizeof(val));
            
            RegCloseKey(hKey);
        }
    }

    static void disableNagle() {
        HKEY hKey;
        const char* basePath = "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces";
        
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, basePath, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            DWORD index = 0;
            char subKey[256];
            DWORD subKeyLen;

            while (true) {
                subKeyLen = sizeof(subKey);
                if (RegEnumKeyExA(hKey, index++, subKey, &subKeyLen, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                    break;

                std::string fullPath = std::string(basePath) + "\\" + subKey;
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

    static void optimizeAll() {
        flushDNS();
        flushARP();
        optimizeTCP();
        disableNagle();
    }

    static void openNetworkSettings() {
        system("start ms-settings:network");
    }
};

}
