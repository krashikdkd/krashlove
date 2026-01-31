#pragma once

#include "types.hpp"
#include "console.hpp"
#include "system.hpp"
#include <iomanip>
#include <sstream>

namespace krash {

class UI {
private:
    Console& con;

public:
    UI(Console& c) : con(c) {}

    void drawLogo() {
        con.setColor(Color::BrightGreen);
        std::cout << R"(
    +==============================================================================+
    |                                                                              |
    |   ██ ▄█▀ ██▀███   ▄████▄   ██░ ██  ██▓     ▒█████   ██▒   █▓▓█████           |
    |   ██▄█▒ ▓██ ▒ ██▒▒██▀ ▀█  ▓██░ ██▒▓██▒    ▒██▒  ██▒▓██░   █▒▓█   ▀           |
    |  ▓███▄░ ▓██ ░▄█ ▒▒▓█    ▄ ▒██▀▀██░▒██░    ▒██░  ██▒ ▓██  █▒░▒███             |
    |  ▓██ █▄ ▒██▀▀█▄  ▒▓▓▄ ▄██▒░▓█ ░██ ▒██░    ▒██   ██░  ▒██ █░░▒▓█  ▄           |
    |  ▒██▒ █▄░██▓ ▒██▒▒ ▓███▀ ░░▓█▒░██▓░██████▒░ ████▓▒░   ▒▀█░  ░▒████▒          |
    |  ▒ ▒▒ ▓▒░ ▒▓ ░▒▓░░ ░▒ ▒  ░ ▒ ░░▒░▒░ ▒░▓  ░░ ▒░▒░▒░    ░ ▐░  ░░ ▒░ ░          |
    |  ░ ░▒ ▒░  ░▒ ░ ▒░  ░  ▒    ▒ ░▒░ ░░ ░ ▒  ░  ░ ▒ ▒░    ░ ░░   ░ ░  ░          |
    |  ░ ░░ ░   ░░   ░ ░         ░  ░░ ░  ░ ░   ░ ░ ░ ▒       ░░     ░             |
    |  ░  ░      ░     ░ ░       ░  ░  ░    ░  ░    ░ ░        ░     ░  ░          |
    |                                                                              |)" << "\n";

        con.setColor(Color::Black, Color::BrightGreen);
        std::cout << "    |              [ ULTIMATE SYSTEM OPTIMIZER v3.0 ]                           |";
        con.setColor(Color::BrightGreen);

        std::cout << R"(
    |                                                                              |
    |             Telegram: @vorkernft  |  GitHub: github.com/krashikdkd           |
    +==============================================================================+
)" << "\n";
        con.reset();
    }

    void drawSystemInfo(const SystemStats& s) {
        con.setColor(Color::BrightCyan);
        std::cout << "    +------------------------------ SYSTEM --------------------------------+\n";

        auto line = [&](const char* label, const std::string& value) {
            con.setColor(Color::BrightWhite);
            std::cout << "    | ";
            con.setColor(Color::BrightYellow);
            std::cout << std::left << std::setw(12) << label;
            con.setColor(Color::White);
            std::string v = value.length() > 55 ? value.substr(0, 55) : value;
            std::cout << std::left << std::setw(55) << v;
            con.setColor(Color::BrightCyan);
            std::cout << " |\n";
        };

        line("CPU:", s.cpuName);
        line("GPU:", s.gpuName);
        line("OS:", s.osName);
        line("Motherboard:", s.motherboard);

        con.setColor(Color::BrightWhite);
        std::cout << "    | ";
        con.setColor(Color::BrightYellow);
        std::cout << std::left << std::setw(12) << "RAM:";
        con.setColor(Color::White);

        std::stringstream ramInfo;
        ramInfo << (s.totalRam - s.availableRam) << "/" << s.totalRam << " MB ";
        std::cout << std::left << std::setw(18) << ramInfo.str();

        std::cout << "[";
        int barW = 22, used = s.ramUsagePercent * barW / 100;
        if (s.ramUsagePercent > 80) con.setColor(Color::BrightRed);
        else if (s.ramUsagePercent > 60) con.setColor(Color::BrightYellow);
        else con.setColor(Color::BrightGreen);
        for (int i = 0; i < used; i++) std::cout << "#";
        con.setColor(Color::Gray);
        for (int i = used; i < barW; i++) std::cout << "-";
        con.setColor(Color::White);
        std::cout << "] " << std::setw(3) << s.ramUsagePercent << "%";
        con.setColor(Color::BrightCyan);
        std::cout << "   |\n";

        std::stringstream diskInfo;
        diskInfo << s.diskFree << " GB / " << s.diskTotal << " GB free";
        line("Disk C:", diskInfo.str());

        std::stringstream procInfo;
        procInfo << s.processCount << " processes | " << s.cpuCores << " CPU cores";
        line("System:", procInfo.str());

        con.setColor(Color::BrightCyan);
        std::cout << "    +------------------------------------------------------------------------+\n";
        con.reset();
    }

    void drawGameModeStatus(bool enabled) {
        if (enabled) {
            con.setColor(Color::Black, Color::BrightGreen);
            std::cout << "    [                      GAME MODE: ACTIVE                              ]";
            con.reset();
            std::cout << "\n";
        } else {
            con.setColor(Color::Gray);
            std::cout << "    [                      Game Mode: inactive                            ]\n";
            con.reset();
        }
    }

    void drawMenu(bool gameModeOn = false) {
        con.setColor(Color::BrightGreen);
        std::cout << "    +------------------------------- MENU ---------------------------------+\n";

        std::string gmText = gameModeOn ? "Game Mode [ON] - press to disable" : "Game Mode [OFF] - press to enable";

        struct MenuEntry { char key; std::string text; Color color; };
        MenuEntry items[] = {
            {'1', "Clean TEMP files", Color::BrightWhite},
            {'2', "Clean browser cache (all browsers)", Color::BrightWhite},
            {'3', "Clean Prefetch + Superfetch", Color::BrightWhite},
            {'4', "Clean Windows cache + logs", Color::BrightWhite},
            {'5', "Clean GPU shader cache (NVIDIA/AMD)", Color::BrightWhite},
            {'6', "Empty Recycle Bin", Color::BrightWhite},
            {'7', "Deep RAM optimization", Color::BrightWhite},
            {'8', gmText, gameModeOn ? Color::BrightGreen : Color::BrightWhite},
            {'9', "Kill background apps", Color::BrightWhite},
            {'S', "Manage startup programs", Color::BrightWhite},
            {'N', "Network optimization (TCP/DNS)", Color::BrightWhite},
            {'P', "Power plan: High Performance", Color::BrightWhite},
            {'D', "Windows Disk Cleanup", Color::BrightWhite},
            {'W', "Open Windows Services", Color::BrightWhite},
            {'A', "=== FULL SYSTEM OPTIMIZATION ===", Color::BrightGreen},
            {'R', "Refresh system info", Color::Gray},
            {'0', "Exit", Color::BrightRed}
        };

        for (auto& item : items) {
            con.setColor(Color::BrightGreen);
            std::cout << "    |  ";

            if (item.key == 'A') {
                con.setColor(Color::Black, Color::BrightGreen);
                std::cout << " [" << item.key << "] " << item.text << " ";
                con.setColor(Color::BrightGreen);
                int pad = 62 - (int)item.text.length();
                for (int p = 0; p < pad; p++) std::cout << " ";
            } else if (item.key == '8' && gameModeOn) {
                con.setColor(Color::Black, Color::BrightGreen);
                std::cout << "[" << item.key << "] " << std::left << std::setw(62) << item.text;
                con.setColor(Color::BrightGreen);
            } else {
                con.setColor(item.color);
                std::cout << "[" << item.key << "] " << std::left << std::setw(62) << item.text;
            }

            con.setColor(Color::BrightGreen);
            std::cout << "  |\n";
        }

        std::cout << "    +------------------------------------------------------------------------+\n";
        con.reset();
    }

    void msg(const std::string& text, bool ok = true) {
        con.setColor(ok ? Color::BrightGreen : Color::BrightRed);
        std::cout << "    " << (ok ? "[+] " : "[!] ") << text << "\n";
        con.reset();
    }

    void progress(const std::string& task, int cur, int total) {
        con.setColor(Color::BrightCyan);
        std::cout << "    [" << cur << "/" << total << "] " << task << "...\n";
        con.reset();
    }

    void wait() {
        con.setColor(Color::Gray);
        std::cout << "\n    Press any key...";
        con.reset();
        con.key();
    }

    char choice() {
        con.setColor(Color::BrightGreen);
        std::cout << "\n    > ";
        con.setColor(Color::BrightWhite);
        char ch = con.key();
        if (ch >= 'a' && ch <= 'z') ch -= 32;
        std::cout << ch << "\n";
        con.reset();
        return ch;
    }
};

}
