#include <iostream>
#include <string>
#include "CPU.h"

void showMenu() {
    std::cout << "\n==== Assembly Simulator ====\n";
    std::cout << "1. Instruction Entry Options\n";
    std::cout << "2. Data Entry Options\n";
    std::cout << "3. Run assembly simulator\n";
    std::cout << "0. Exit\n";
    std::cout << "\nEnter your choice: ";
}

void dataEntryOptions(Memory* memory, uint32_t& nextAddress) {
    // std::cout << "\n1. Manual variable entry\n";
    // std::cout << "2. Load variables from file\n";
    // std::cout << "3. Go back to main menu\n";
    int choice;
    std::cin >> choice;
    switch (choice) {
        case 1:
            memory->manualVariableInput(nextAddress);
            break;
        case 2:
            memory->loadVariablesFromFile(nextAddress);
            break;
        case 3:
            return; // Go back to main menu
        default:
            std::cout << "Invalid choice, try again.\n";
    }
}

void InstructionEntryOptions(Memory* memory) {
    // std::cout << "\n1. Insert instructions manually\n";
    // std::cout << "2. Load instructions from file\n";
    // std::cout << "3. Go back to main menu\n";
    int choice;
    std::cin >> choice;
    switch (choice) {
        case 1:
            memory->insertInstructionsManually();
            break;
        case 2:
            memory->loadInstructionsFromFile();
            break;
        case 3:
            return; // Go back to main menu
        default:
            std::cout << "Invalid choice, try again.\n";
    }
}

int main() {
    // Initialize CPU and memory with default files
    // You can change these filenames as needed
    const std::string codeFile = "fib.txt";   
    const std::string dataFile = "data.txt";
    CPU cpu(codeFile, dataFile);

    uint32_t nextVarAddress = 0;
    bool running = true;
    while (running) {
        // showMenu();
        int choice = -1;
        std::cin >> choice;
        switch (choice) {
            case 1:
                InstructionEntryOptions(cpu.getMemory());
                break;
            case 2:
                dataEntryOptions(cpu.getMemory(), nextVarAddress);
                break;
            case 3: {
                // std::cout << "Running assembly simulator...\n";
                cpu.run();
                // Save data on exit (optional)
                cpu.getMemory()->saveDataToFile(dataFile);
                running = false;
                break;
            }
            case 0:
                running = false;
                break;
            default:
                std::cout << "Invalid choice, try again.\n";
        }
    }
    // std::cout << "Exiting simulator.\n";
    return 0;
}