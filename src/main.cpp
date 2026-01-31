#include "../include/optimizer.h"
#include <iostream>
#include <string>
#include <limits>

using namespace KrashLove;

void PrintBanner() {
    std::cout << R"(
╔═══════════════════════════════════════════════════════════╗
║                                                           ║
║           K R A S H   L O V E   O P T I M I Z E R        ║
║                                                           ║
║         Hardcore Windows Performance Optimizer           ║
║                   C++17 | Console App                    ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝
)" << std::endl;
}

void PrintMenu() {
    std::cout << "\n╔═══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║           OPTIMIZATION MENU                   ║" << std::endl;
    std::cout << "╠═══════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  1. System Cleanup (Temp/Prefetch/RecycleBin)║" << std::endl;
    std::cout << "║  2. RAM Optimization (Memory Clear/Working Set)║" << std::endl;
    std::cout << "║  3. CPU Priority Boost                        ║" << std::endl;
    std::cout << "║  4. Network (TCP) Optimization                ║" << std::endl;
    std::cout << "║  5. Background Process Management             ║" << std::endl;
    std::cout << "║  6. Run All Optimizations                     ║" << std::endl;
    std::cout << "║  7. Display System Status                     ║" << std::endl;
    std::cout << "║  0. Exit                                      ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════════════╝" << std::endl;
    std::cout << "\nSelect option: ";
}

void RunSystemCleanup() {
    std::cout << "\n=== SYSTEM CLEANUP ===" << std::endl;
    
    SystemCleanup::DisplayCleanupStatus();
    
    std::cout << "\nProceed with cleanup? (y/n): ";
    char confirm;
    std::cin >> confirm;
    
    if (confirm == 'y' || confirm == 'Y') {
        SystemCleanup::CleanTempFiles();
        SystemCleanup::CleanPrefetchFiles();
        SystemCleanup::EmptyRecycleBin();
        Utils::PrintSuccess("System cleanup completed!");
    } else {
        Utils::PrintInfo("Cleanup cancelled");
    }
}

void RunRAMOptimization() {
    std::cout << "\n=== RAM OPTIMIZATION ===" << std::endl;
    
    RAMOptimizer::DisplayMemoryStatus();
    
    std::cout << "\nProceed with RAM optimization? (y/n): ";
    char confirm;
    std::cin >> confirm;
    
    if (confirm == 'y' || confirm == 'Y') {
        RAMOptimizer::OptimizeRAM();
        std::cout << "\nAfter optimization:" << std::endl;
        RAMOptimizer::DisplayMemoryStatus();
        Utils::PrintSuccess("RAM optimization completed!");
    } else {
        Utils::PrintInfo("RAM optimization cancelled");
    }
}

void RunCPUOptimization() {
    std::cout << "\n=== CPU OPTIMIZATION ===" << std::endl;
    std::cout << "1. Boost specific process priority" << std::endl;
    std::cout << "2. Set system priority" << std::endl;
    std::cout << "3. Optimize CPU core parking" << std::endl;
    std::cout << "4. List running processes" << std::endl;
    std::cout << "\nSelect option: ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1: {
            std::cout << "Enter process name (e.g., chrome.exe): ";
            std::wstring processName;
            std::wcin.ignore();
            std::getline(std::wcin, processName);
            CPUOptimizer::BoostProcessPriority(processName);
            break;
        }
        case 2:
            CPUOptimizer::SetSystemPriority();
            break;
        case 3:
            CPUOptimizer::OptimizeCoreParking();
            Utils::PrintInfo("Note: Restart required for changes to take effect");
            break;
        case 4:
            CPUOptimizer::ListRunningProcesses();
            break;
        default:
            Utils::PrintError("Invalid option");
    }
}

void RunNetworkOptimization() {
    std::cout << "\n=== NETWORK OPTIMIZATION ===" << std::endl;
    
    NetworkOptimizer::DisplayNetworkStatus();
    
    std::cout << "\nProceed with TCP optimization? (y/n): ";
    char confirm;
    std::cin >> confirm;
    
    if (confirm == 'y' || confirm == 'Y') {
        NetworkOptimizer::OptimizeTCP();
        Utils::PrintSuccess("Network optimization completed!");
        Utils::PrintInfo("Note: Restart required for changes to take effect");
    } else {
        Utils::PrintInfo("Network optimization cancelled");
    }
}

void RunProcessManagement() {
    std::cout << "\n=== BACKGROUND PROCESS MANAGEMENT ===" << std::endl;
    std::cout << "1. Disable unnecessary services" << std::endl;
    std::cout << "2. List all services" << std::endl;
    std::cout << "3. Disable specific service" << std::endl;
    std::cout << "4. Enable specific service" << std::endl;
    std::cout << "\nSelect option: ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            ProcessManager::DisableUnnecessaryServices();
            Utils::PrintInfo("Note: Service changes will take effect after restart");
            break;
        case 2:
            ProcessManager::ListServices();
            break;
        case 3: {
            std::cout << "Enter service name: ";
            std::wstring serviceName;
            std::wcin.ignore();
            std::getline(std::wcin, serviceName);
            if (ProcessManager::DisableService(serviceName)) {
                Utils::PrintSuccess("Service disabled");
            } else {
                Utils::PrintError("Failed to disable service");
            }
            break;
        }
        case 4: {
            std::cout << "Enter service name: ";
            std::wstring serviceName;
            std::wcin.ignore();
            std::getline(std::wcin, serviceName);
            if (ProcessManager::EnableService(serviceName)) {
                Utils::PrintSuccess("Service enabled");
            } else {
                Utils::PrintError("Failed to enable service");
            }
            break;
        }
        default:
            Utils::PrintError("Invalid option");
    }
}

void RunAllOptimizations() {
    std::cout << "\n=== RUNNING ALL OPTIMIZATIONS ===" << std::endl;
    
    Utils::PrintInfo("WARNING: This will apply all optimizations!");
    std::cout << "Are you sure? (y/n): ";
    char confirm;
    std::cin >> confirm;
    
    if (confirm != 'y' && confirm != 'Y') {
        Utils::PrintInfo("Cancelled");
        return;
    }
    
    // System Cleanup
    std::cout << "\n[1/5] System Cleanup..." << std::endl;
    SystemCleanup::CleanTempFiles();
    SystemCleanup::CleanPrefetchFiles();
    SystemCleanup::EmptyRecycleBin();
    
    // RAM Optimization
    std::cout << "\n[2/5] RAM Optimization..." << std::endl;
    RAMOptimizer::OptimizeRAM();
    
    // CPU Optimization
    std::cout << "\n[3/5] CPU Optimization..." << std::endl;
    CPUOptimizer::SetSystemPriority();
    CPUOptimizer::OptimizeCoreParking();
    
    // Network Optimization
    std::cout << "\n[4/5] Network Optimization..." << std::endl;
    NetworkOptimizer::OptimizeTCP();
    
    // Process Management
    std::cout << "\n[5/5] Process Management..." << std::endl;
    ProcessManager::DisableUnnecessaryServices();
    
    Utils::PrintSuccess("\n╔════════════════════════════════════════════╗");
    Utils::PrintSuccess("║  ALL OPTIMIZATIONS COMPLETED SUCCESSFULLY  ║");
    Utils::PrintSuccess("╚════════════════════════════════════════════╝");
    Utils::PrintInfo("Note: Restart your system for all changes to take effect");
}

void DisplaySystemStatus() {
    std::cout << "\n=== SYSTEM STATUS ===" << std::endl;
    
    RAMOptimizer::DisplayMemoryStatus();
    NetworkOptimizer::DisplayNetworkStatus();
    SystemCleanup::DisplayCleanupStatus();
}

int main() {
    // Check for administrator privileges
    if (!Utils::IsRunningAsAdmin()) {
        Utils::PrintError("This program requires administrator privileges!");
        std::cout << "\nAttempting to restart with admin rights...\n";
        
        if (Utils::RestartAsAdmin()) {
            return 0;
        } else {
            Utils::PrintError("Failed to restart with admin rights. Please run as administrator manually.");
            Utils::WaitForKeyPress();
            return 1;
        }
    }
    
    PrintBanner();
    Utils::PrintSuccess("Running with administrator privileges");
    
    bool running = true;
    while (running) {
        PrintMenu();
        
        int choice;
        std::cin >> choice;
        
        // Clear input buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                RunSystemCleanup();
                break;
            case 2:
                RunRAMOptimization();
                break;
            case 3:
                RunCPUOptimization();
                break;
            case 4:
                RunNetworkOptimization();
                break;
            case 5:
                RunProcessManagement();
                break;
            case 6:
                RunAllOptimizations();
                break;
            case 7:
                DisplaySystemStatus();
                break;
            case 0:
                Utils::PrintInfo("Exiting KrashLove Optimizer. Stay optimized!");
                running = false;
                break;
            default:
                Utils::PrintError("Invalid option. Please try again.");
        }
        
        if (running) {
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
    }
    
    return 0;
}
