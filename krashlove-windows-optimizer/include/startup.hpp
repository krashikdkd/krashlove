#pragma once

#include "types.hpp"
#include <algorithm>

namespace krash {

class Startup {
public:
    static std::vector<std::pair<std::string, std::string>> getItems() {
        std::vector<std::pair<std::string, std::string>> items;
        
        const char* keys[] = {
            "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
            "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"
        };
        
        HKEY roots[] = {HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE};
        
        for (HKEY root : roots) {
            for (const char* keyPath : keys) {
                HKEY hKey;
                if (RegOpenKeyExA(root, keyPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
                    DWORD index = 0;
                    char name[256], value[512];
                    DWORD nameLen, valueLen, type;

                    while (true) {
                        nameLen = sizeof(name);
                        valueLen = sizeof(value);
                        if (RegEnumValueA(hKey, index++, name, &nameLen, NULL, &type, (BYTE*)value, &valueLen) != ERROR_SUCCESS)
                            break;
                        if (type == REG_SZ || type == REG_EXPAND_SZ) {
                            items.push_back({name, value});
                        }
                    }
                    RegCloseKey(hKey);
                }
            }
        }
        
        return items;
    }

    static bool disableItem(const std::string& name) {
        HKEY hKey;
        bool success = false;

        if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
            if (RegDeleteValueA(hKey, name.c_str()) == ERROR_SUCCESS) success = true;
            RegCloseKey(hKey);
        }

        return success;
    }

    static int getCount() {
        return (int)getItems().size();
    }

    static void openTaskManager() {
        system("taskmgr /0 /startup");
    }
};

}
