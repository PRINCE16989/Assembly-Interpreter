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

void dataEntryOptions(Memory* memory, uint32_t& nextAddress, std::string& dataFile) {
    // std::cout << "\n1. Manual variable entry\n";
    // std::cout << "2. Load variables from file\n";
    // std::cout << "3. Go back to main menu\n";
    int choice;
    std::cin >> choice;
    switch (choice) {
        case 1:
            memory->manualVariableInput(nextAddress, dataFile);
            break;
        case 2:
            memory->loadVariablesFromFile(nextAddress, dataFile);
            break;
        case 3:
            return; // Go back to main menu
        default:
            std::cout << "Invalid choice, try again.\n";
    }
}

void InstructionEntryOptions(Memory* memory, std::string &codeFile) {
    // std::cout << "\n1. Insert instructions manually\n";
    // std::cout << "2. Load instructions from file\n";
    // std::cout << "3. Go back to main menu\n";
    int choice;
    std::cin >> choice;
    switch (choice) {
        case 1:
            memory->insertInstructionsManually(codeFile);
            break;
        case 2:
            memory->loadInstructionsFromFile(codeFile);
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
    std::string codeFile = "default.txt";   
    std::string dataFile = "default_data.txt";
    std::string outputFile = "output.txt";
    // CPU cpu(codeFile, dataFile);
    // Alternatively, you can use the default constructor for non-interactive mode
    CPU cpu;

    uint32_t nextVarAddress = 0;
    bool running = true;
    while (running) {
        // showMenu();
        int choice = -1;
        std::cin >> choice;
        switch (choice) {
            case 1:
                InstructionEntryOptions(cpu.getMemory(), codeFile);
                break;
            case 2:
                dataEntryOptions(cpu.getMemory(), nextVarAddress, dataFile);
                break;
            case 3: {
                // std::cout << "Running assembly simulator...\n";
                cpu.run();
                // Save data on exit (optional)
                // can save data in input_data file itself
                // saved in output.txt for running testcases.
                cpu.getMemory()->saveDataToFile(outputFile);
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
