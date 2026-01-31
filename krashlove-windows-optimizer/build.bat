@echo off
echo Building KRASHLOVE...
g++ -std=c++17 -O2 -static -o bin\KRASHLOVE.exe -Iinclude src\main.cpp -lshell32 -ladvapi32 -luser32 -lgdi32 -lpsapi
if %errorlevel% == 0 (
    echo Build successful: bin\KRASHLOVE.exe
) else (
    echo Build failed
)
pause
