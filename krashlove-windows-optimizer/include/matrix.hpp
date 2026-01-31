#pragma once

#include "types.hpp"
#include "console.hpp"
#include <random>
#include <chrono>

namespace krash {

class Matrix {
private:
    Console& con;
    std::mt19937 rng;

public:
    Matrix(Console& console) : con(console) {
        std::random_device rd;
        rng.seed(rd());
    }

    void run(int durationMs = 2500) {
        con.hideCursor();
        con.clear();

        int width = con.width();
        int height = con.height();

        std::vector<int> drops(width, 0);
        std::vector<int> speeds(width);
        std::vector<int> lengths(width);

        std::uniform_int_distribution<> speedDist(1, 4);
        std::uniform_int_distribution<> charDist(33, 126);
        std::uniform_int_distribution<> initDist(0, height);
        std::uniform_int_distribution<> lenDist(5, 15);

        for (int i = 0; i < width; i++) {
            drops[i] = height - initDist(rng);
            speeds[i] = speedDist(rng);
            lengths[i] = lenDist(rng);
        }

        auto start = std::chrono::steady_clock::now();
        int frame = 0;

        while (true) {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start).count();

            if (elapsed > durationMs) break;
            if (con.keyPressed()) { con.key(); break; }

            for (int x = 0; x < width - 1; x += 2) {
                if (frame % speeds[x] == 0) {
                    drops[x]--;

                    if (drops[x] < -lengths[x]) {
                        drops[x] = height - 1;
                        speeds[x] = speedDist(rng);
                        lengths[x] = lenDist(rng);
                    }

                    if (drops[x] >= 0 && drops[x] < height) {
                        con.setCursor(x, drops[x]);
                        con.setColor(Color::BrightWhite);
                        std::cout << (char)charDist(rng);
                    }

                    for (int t = 1; t < lengths[x] && drops[x] + t < height; t++) {
                        if (drops[x] + t >= 0) {
                            con.setCursor(x, drops[x] + t);
                            if (t < 2) con.setColor(Color::BrightGreen);
                            else if (t < 4) con.setColor(Color::Green);
                            else if (t < lengths[x] - 2) con.setColor(Color::Green);
                            else con.setColor(Color::Black);
                            std::cout << (char)charDist(rng);
                        }
                    }
                }
            }

            frame++;
            Sleep(25);
        }

        con.showCursor();
        con.clear();
    }
};

}
