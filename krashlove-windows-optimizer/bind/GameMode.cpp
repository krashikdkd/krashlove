#include <windows.h>
#include <string>
#include <fstream>

std::string getStatePath() {
    char path[MAX_PATH];
    GetTempPathA(MAX_PATH, path);
    return std::string(path) + "krashlove_gamemode.state";
}

bool isGameModeEnabled() {
    std::ifstream f(getStatePath());
    if (!f.is_open()) return false;
    int state = 0;
    f >> state;
    return state == 1;
}

void saveState(bool enabled) {
    std::ofstream f(getStatePath());
    f << (enabled ? 1 : 0);
}

void enableGameMode() {
    HANDLE hProcess = GetCurrentProcess();
    SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS);

    HKEY hKey;
    DWORD value = 1;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, "EnableTransparency", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        RegCloseKey(hKey);
    }

    DWORD disable = 0;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\DWM", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, "EnableAeroPeek", 0, REG_DWORD, (BYTE*)&disable, sizeof(disable));
        RegCloseKey(hKey);
    }

    SystemParametersInfoA(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, 0);
    SystemParametersInfoA(SPI_SETMENUANIMATION, 0, (void*)FALSE, 0);
    SystemParametersInfoA(SPI_SETCOMBOBOXANIMATION, 0, (void*)FALSE, 0);
    SystemParametersInfoA(SPI_SETLISTBOXSMOOTHSCROLLING, 0, (void*)FALSE, 0);

    system("powercfg /setactive 8c5e7fda-e8bf-4a96-9a85-a6e23a8c635c >nul 2>&1");

    saveState(true);
}

void disableGameMode() {
    HANDLE hProcess = GetCurrentProcess();
    SetPriorityClass(hProcess, NORMAL_PRIORITY_CLASS);

    HKEY hKey;
    DWORD enable = 1;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\DWM", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, "EnableAeroPeek", 0, REG_DWORD, (BYTE*)&enable, sizeof(enable));
        RegCloseKey(hKey);
    }

    SystemParametersInfoA(SPI_SETDRAGFULLWINDOWS, TRUE, NULL, 0);
    SystemParametersInfoA(SPI_SETMENUANIMATION, 0, (void*)TRUE, 0);
    SystemParametersInfoA(SPI_SETCOMBOBOXANIMATION, 0, (void*)TRUE, 0);
    SystemParametersInfoA(SPI_SETLISTBOXSMOOTHSCROLLING, 0, (void*)TRUE, 0);

    system("powercfg /setactive 381b4222-f694-41f0-9685-ff5bb260df2e >nul 2>&1");

    saveState(false);
}

void showNotification(const wchar_t* title, const wchar_t* msg, bool success) {
    MessageBoxW(NULL, msg, title, MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    std::string args(lpCmdLine);

    if (args.find("on") != std::string::npos || args.find("ON") != std::string::npos || 
        args.find("-on") != std::string::npos || args.find("/on") != std::string::npos) {
        enableGameMode();
        showNotification(L"KRASHLOVE", L"🎮 GAME MODE: ВКЛЮЧЁН\n\n• CPU Priority: HIGH\n• Анимации: OFF\n• Power Plan: High Performance", true);
    }
    else if (args.find("off") != std::string::npos || args.find("OFF") != std::string::npos ||
             args.find("-off") != std::string::npos || args.find("/off") != std::string::npos) {
        disableGameMode();
        showNotification(L"KRASHLOVE", L"💤 GAME MODE: ВЫКЛЮЧЕН\n\n• CPU Priority: NORMAL\n• Анимации: ON\n• Power Plan: Balanced", false);
    }
    else {
        if (isGameModeEnabled()) {
            disableGameMode();
            showNotification(L"KRASHLOVE", L"💤 GAME MODE: ВЫКЛЮЧЕН\n\n• CPU Priority: NORMAL\n• Анимации: ON\n• Power Plan: Balanced", false);
        } else {
            enableGameMode();
            showNotification(L"KRASHLOVE", L"🎮 GAME MODE: ВКЛЮЧЁН\n\n• CPU Priority: HIGH\n• Анимации: OFF\n• Power Plan: High Performance", true);
        }
    }

    return 0;
}
