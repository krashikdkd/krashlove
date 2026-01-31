#pragma once

#include "types.hpp"
#include <iostream>
#include <conio.h>

namespace krash {

class Console {
private:
    HANDLE hOut;
    HANDLE hIn;

public:
    Console() {
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        hIn = GetStdHandle(STD_INPUT_HANDLE);
        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
        SetConsoleTitleA("KRASHLOVE v3.0 - Ultimate System Optimizer");

        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 0;
        cfi.dwFontSize.Y = 16;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcscpy_s(cfi.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    }

    void setColor(Color fg, Color bg = Color::Black) {
        SetConsoleTextAttribute(hOut, (WORD)fg | ((WORD)bg << 4));
    }

    void reset() { SetConsoleTextAttribute(hOut, 7); }

    void setCursor(int x, int y) {
        COORD pos = {(SHORT)x, (SHORT)y};
        SetConsoleCursorPosition(hOut, pos);
    }

    void hideCursor() {
        CONSOLE_CURSOR_INFO info = {100, FALSE};
        SetConsoleCursorInfo(hOut, &info);
    }

    void showCursor() {
        CONSOLE_CURSOR_INFO info = {100, TRUE};
        SetConsoleCursorInfo(hOut, &info);
    }

    void clear() { system("cls"); }

    int width() {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    int height() {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }

    void resize(int w, int h) {
        COORD bufferSize = {(SHORT)w, (SHORT)h};
        SetConsoleScreenBufferSize(hOut, bufferSize);
        SMALL_RECT rect = {0, 0, (SHORT)(w - 1), (SHORT)(h - 1)};
        SetConsoleWindowInfo(hOut, TRUE, &rect);
    }

    char key() { return _getch(); }
    bool keyPressed() { return _kbhit(); }
};

}
