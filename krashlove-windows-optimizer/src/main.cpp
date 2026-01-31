#include <windows.h>
#include <iostream>
#include <shellapi.h>

#include "types.hpp"
#include "console.hpp"
#include "system.hpp"
#include "cleaner.hpp"
#include "memory.hpp"
#include "gamemode.hpp"
#include "startup.hpp"
#include "network.hpp"
#include "matrix.hpp"
#include "ui.hpp"

using namespace krash;

bool isRunAsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuth = SECURITY_NT_AUTHORITY;
    
    if (AllocateAndInitializeSid(&ntAuth, 2, SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    return isAdmin == TRUE;
}

void restartAsAdmin(int argc, char* argv[]) {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    
    std::string args;
    for (int i = 1; i < argc; i++) {
        if (i > 1) args += " ";
        args += argv[i];
    }
    
    ShellExecuteA(NULL, "runas", path, args.c_str(), NULL, SW_SHOWNORMAL);
}

void fullOptimization(UI& ui) {
    ui.progress("Cleaning TEMP files", 1, 10);
    auto r1 = Cleaner::cleanTemp();
    ui.msg("TEMP: " + std::to_string(r1.filesDeleted) + " files");
    Sleep(200);

    ui.progress("Cleaning browser cache", 2, 10);
    auto r2 = Cleaner::cleanBrowsers();
    ui.msg("Browsers: " + std::to_string(r2.filesDeleted) + " files");
    Sleep(200);

    ui.progress("Cleaning Prefetch", 3, 10);
    auto r3 = Cleaner::cleanPrefetch();
    ui.msg("Prefetch: " + std::to_string(r3.filesDeleted) + " files");
    Sleep(200);

    ui.progress("Cleaning Windows cache", 4, 10);
    auto r4 = Cleaner::cleanWindows();
    ui.msg("Windows: " + std::to_string(r4.filesDeleted) + " files");
    Sleep(200);

    ui.progress("Cleaning GPU shader cache", 5, 10);
    auto r5 = Cleaner::cleanNvidia();
    auto r6 = Cleaner::cleanAMD();
    ui.msg("GPU cache: " + std::to_string(r5.filesDeleted + r6.filesDeleted) + " files");
    Sleep(200);

    ui.progress("Deep RAM optimization", 6, 10);
    size_t freed = Memory::deepClean();
    ui.msg("RAM freed: " + std::to_string(freed) + " MB");
    Sleep(200);

    ui.progress("Enabling Game Mode", 7, 10);
    GameMode::enable();
    ui.msg("Game Mode: ON");
    Sleep(200);

    ui.progress("Killing background apps", 8, 10);
    int killed = GameMode::killBackgroundApps();
    ui.msg("Killed: " + std::to_string(killed) + " processes");
    Sleep(200);

    ui.progress("Optimizing network", 9, 10);
    Network::optimizeAll();
    ui.msg("Network: optimized");
    Sleep(200);

    ui.progress("Setting High Performance power plan", 10, 10);
    GameMode::setHighPerformance();
    ui.msg("Power plan: High Performance");

    std::cout << "\n";
    ui.msg("========== OPTIMIZATION COMPLETE ==========");

    int total = r1.filesDeleted + r2.filesDeleted + r3.filesDeleted + r4.filesDeleted + r5.filesDeleted + r6.filesDeleted;
    ui.msg("Total cleaned: " + std::to_string(total) + " files");
    ui.msg("RAM freed: " + std::to_string(freed) + " MB");
}

void showQuickNotification(bool enabled) {
    if (enabled) {
        MessageBoxA(NULL, 
            "GAME MODE: ON\n\n"
            "+ CPU Priority: HIGH\n"
            "+ Animations: OFF\n"
            "+ Power Plan: High Performance",
            "KRASHLOVE", MB_OK | MB_ICONINFORMATION);
    } else {
        MessageBoxA(NULL, 
            "GAME MODE: OFF\n\n"
            "+ CPU Priority: NORMAL\n"
            "+ Animations: ON\n"
            "+ Power Plan: Balanced",
            "KRASHLOVE", MB_OK | MB_ICONINFORMATION);
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string arg(argv[1]);
        
        if (arg == "on" || arg == "ON" || arg == "-on" || arg == "/on") {
            GameMode::enable();
            showQuickNotification(true);
            return 0;
        }
        else if (arg == "off" || arg == "OFF" || arg == "-off" || arg == "/off") {
            GameMode::disable();
            showQuickNotification(false);
            return 0;
        }
        else if (arg == "toggle" || arg == "TOGGLE" || arg == "-toggle" || arg == "/toggle" ||
                 arg == "gamemode" || arg == "gm" || arg == "-gm") {
            if (GameMode::isEnabled()) {
                GameMode::disable();
                showQuickNotification(false);
            } else {
                GameMode::enable();
                showQuickNotification(true);
            }
            return 0;
        }
        else if (arg == "admin" || arg == "-admin" || arg == "/admin") {
        }
    }

    if (!isRunAsAdmin()) {
        int result = MessageBoxA(NULL, 
            "KRASHLOVE works better with admin rights.\n\n"
            "Restart as Administrator?",
            "KRASHLOVE", MB_YESNO | MB_ICONQUESTION);
        
        if (result == IDYES) {
            restartAsAdmin(argc, argv);
            return 0;
        }
    }

    Console con;
    con.resize(86, 50);

    Matrix matrix(con);
    matrix.run(2000);

    UI ui(con);
    bool running = true;

    while (running) {
        con.clear();
        ui.drawLogo();
        ui.drawSystemInfo(System::getStats());
        ui.drawGameModeStatus(GameMode::isEnabled());
        ui.drawMenu(GameMode::isEnabled());

        char ch = ui.choice();

        switch (ch) {
            case '1': {
                ui.msg("Cleaning TEMP...");
                auto r = Cleaner::cleanTemp();
                ui.msg("Deleted: " + std::to_string(r.filesDeleted) + " files");
                ui.wait();
                break;
            }

            case '2': {
                ui.msg("Cleaning browser cache...");
                auto r = Cleaner::cleanBrowsers();
                ui.msg("Deleted: " + std::to_string(r.filesDeleted) + " files");
                ui.wait();
                break;
            }

            case '3': {
                ui.msg("Cleaning Prefetch...");
                auto r = Cleaner::cleanPrefetch();
                ui.msg("Deleted: " + std::to_string(r.filesDeleted) + " files");
                ui.wait();
                break;
            }

            case '4': {
                ui.msg("Cleaning Windows cache...");
                auto r = Cleaner::cleanWindows();
                ui.msg("Deleted: " + std::to_string(r.filesDeleted) + " files");
                ui.wait();
                break;
            }

            case '5': {
                ui.msg("Cleaning GPU shader cache...");
                auto r1 = Cleaner::cleanNvidia();
                auto r2 = Cleaner::cleanAMD();
                ui.msg("Deleted: " + std::to_string(r1.filesDeleted + r2.filesDeleted) + " files");
                ui.wait();
                break;
            }

            case '6': {
                ui.msg("Emptying Recycle Bin...");
                Cleaner::cleanRecycleBin();
                ui.msg("Recycle Bin emptied");
                ui.wait();
                break;
            }

            case '7': {
                ui.msg("Deep RAM optimization...");
                size_t before = Memory::getAvailable();
                size_t freed = Memory::deepClean();
                size_t after = Memory::getAvailable();
                ui.msg("Before: " + std::to_string(before) + " MB");
                ui.msg("After: " + std::to_string(after) + " MB");
                ui.msg("Freed: " + std::to_string(freed) + " MB");
                ui.wait();
                break;
            }

            case '8': {
                if (GameMode::isEnabled()) {
                    GameMode::disable();
                    ui.msg("Game Mode: DISABLED");
                } else {
                    GameMode::enable();
                    ui.msg("Game Mode: ENABLED");
                    ui.msg("CPU Priority: HIGH");
                    ui.msg("Windows effects: DISABLED");
                }
                ui.wait();
                break;
            }

            case '9': {
                ui.msg("Killing background apps...");
                int killed = GameMode::killBackgroundApps();
                ui.msg("Killed: " + std::to_string(killed) + " processes");
                ui.wait();
                break;
            }

            case 'S': {
                auto items = Startup::getItems();
                con.setColor(Color::BrightYellow);
                std::cout << "\n    ============= STARTUP PROGRAMS =============\n\n";
                con.setColor(Color::White);
                for (auto& item : items) {
                    std::cout << "      * " << item.first << "\n";
                }
                con.setColor(Color::BrightCyan);
                std::cout << "\n    Total: " << items.size() << " programs\n";
                std::cout << "\n    [T] Open Task Manager startup tab\n";
                std::cout << "    [Any] Return to menu\n";
                char c = con.key();
                if (c == 't' || c == 'T') Startup::openTaskManager();
                break;
            }

            case 'N': {
                ui.msg("Optimizing network...");
                Network::optimizeAll();
                ui.msg("DNS cache: flushed");
                ui.msg("ARP cache: flushed");
                ui.msg("TCP settings: optimized");
                ui.msg("Nagle algorithm: disabled");
                ui.wait();
                break;
            }

            case 'P': {
                GameMode::setHighPerformance();
                ui.msg("Power plan: High Performance");
                ui.wait();
                break;
            }

            case 'D': {
                ui.msg("Starting Windows Disk Cleanup...");
                Cleaner::runDiskCleanup();
                break;
            }

            case 'W': {
                ShellExecuteA(NULL, "open", "services.msc", NULL, NULL, SW_SHOW);
                ui.msg("Windows Services opened");
                ui.wait();
                break;
            }

            case 'A': {
                con.clear();
                ui.drawLogo();
                con.setColor(Color::BrightYellow);
                std::cout << "\n    ============== FULL OPTIMIZATION ==============\n\n";
                fullOptimization(ui);
                ui.wait();
                break;
            }

            case 'R':
                break;

            case '0':
            case 27:
                running = false;
                break;
        }
    }

    con.clear();
    con.setColor(Color::BrightGreen);
    std::cout << R"(

    +==============================================================================+
    |                                                                              |
    |                      Thank you for using KRASHLOVE!                          |
    |                                                                              |
    |             Telegram: @vorkernft  |  GitHub: github.com/krashikdkd           |
    |                                                                              |
    +==============================================================================+

)" << "\n";
    con.reset();

    return 0;
}
