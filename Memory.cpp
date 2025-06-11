#include "Memory.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
Memory::Memory(std::string& codeFile, std::string& dataFile) {
    // Load instructions from file
    loadInstructionsUsingFile(codeFile);

    // Load data from file
    loadVariablesFromFile(dataFile);
}

// insert instructions manually, for interactive mode
void Memory::insertInstructionsManually(std::string& codeFile) {
    // std::cout << "Enter instructions (one per line, empty line to finish):\n";
    // remove the above comment in case of interactive mode
    program_.clear(); // Clear existing instructions
    codeFile = "new_instructions.txt"; 
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break; // Stop on empty line
        program_.emplace_back(line);
    }
}

// load instructions from a file specified by the user
void Memory::loadInstructionsFromFile(std::string& codeFile) {
    program_.clear(); // Clear existing instructions
    // std::cout << "Enter the instruction file name: ";
    // remove the above comment in case of interactive mode
    std::string filename;
    std::cin >> filename;
    codeFile = filename; 
    loadInstructionsUsingFile(filename);
}

//load instructions from an existing file
void Memory::loadInstructionsUsingFile(std::string& filename) {
    std::ifstream code(filename);
    if (!code) throw std::runtime_error("Failed to open instruction file.");
    std::string line;
    while (std::getline(code, line)) {
        if (line.empty()) continue; // Skip empty lines
        program_.emplace_back(line); 
    }
    code.close();
    // Optionally, you can check if program_ is empty and throw an error
    // if (program_.empty()) {
    //     throw std::runtime_error("No valid instructions found in the file.");
    // }
}

// Load variables from a file specified by the user
void Memory::loadVariablesFromFile(uint32_t& nextAddress, std::string& dataFile) {
    clear(); // Clear existing variables
    // std::cout << "Enter the data file name: "; 
    // remove the above comment in case of interactive mode
    std::string filename;
    std::cin >> filename;
    std::cin.ignore();
    dataFile = filename; 
    loadVariablesFromFile(filename);
    uint32_t maxAddr = 0;
    for (const auto& name : getVariableNames()) {
        uint32_t addr = getVariableAddress(name);
        if (addr > maxAddr) maxAddr = addr;
    }
    nextAddress = maxAddr + 4;
}

// Load variables from an existing file
void Memory::loadVariablesFromFile(const std::string& dataFile) {
    std::ifstream in(dataFile);
    if(!in) throw std::runtime_error("Failed to open data file.");
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string var;
        uint32_t addr;
        int32_t val;
        ss >> var >> addr >> val;
        symbolTable_[var] = addr;
        data_[addr] = val;
    }
}

// Manual input of variables, for interactive mode
void Memory::manualVariableInput(uint32_t& nextAddress, std::string& dataFile) {
    clear(); // Clear existing variables
    dataFile = "new_data.txt"; // Default output file name
    while(true) {
        //std::cout << "Enter variable name (or 'exit' to stop): ";
        // remove the above comment in case of interactive mode
        std::string varName;
        std::cin >> varName;
        if (varName == "exit") break;

        // std::cout << "Enter variable value: ";
        // remove the above comment in case of interactive mode
        int32_t value;
        std::cin >> value;

        setVariable(varName, nextAddress);
        store(nextAddress, value);
        nextAddress += 4; // Assume word-aligned, 4-byte variables
    }
}

// Fetch an instruction by program counter (PC)
const Instruction& Memory::fetchInstruction(uint32_t pc) const {
    if (pc >= program_.size()) throw std::out_of_range("PC out of range");
    return program_[pc];
}

// Clear all data and symbol table 
void Memory::clear() { data_.clear(); symbolTable_.clear(); }

// Get the number of instructions in the program
size_t Memory::instructionCount() const {
    return program_.size();
}

// Store a value at a specific address
void Memory::store(uint32_t address, int32_t value) {
    data_[address] = value;
}

// Load a value from a specific address
int32_t Memory::load(uint32_t address) const {
    auto it = data_.find(address);
    if (it == data_.end()) throw std::runtime_error("Address not initialized");
    return it->second;
}

// Check if a variable exists in the symbol table
bool Memory::hasVariable(const std::string& name) const {
    return symbolTable_.count(name) > 0;
}

// Get the address of a variable by its name
uint32_t Memory::getVariableAddress(const std::string& name) const {
    auto it = symbolTable_.find(name);
    if (it == symbolTable_.end()) throw std::runtime_error("Variable not found");
    return it->second;
}

// Set a variable in the symbol table with its address
void Memory::setVariable(const std::string& name, uint32_t address) {
    symbolTable_[name] = address;
}

// Save the data memory to a file
void Memory::saveDataToFile(const std::string& dataFile) const {
    std::ofstream out(dataFile);
    std::unordered_map<std::string,bool> visited; 
    for (const auto& [name, addr] : symbolTable_) {
        visited[std::to_string(addr)] = true; 
        out << name << " " << addr << " " << data_.at(addr) << "\n";
    }
    for (const auto& [addr, value] : data_) {
        if (visited[std::to_string(addr)]) continue;
        visited[std::to_string(addr)] = true; 
        out << "var_" << addr << " " << addr << " " << value << "\n"; // Save unnamed variables
    }
}

// Get the names of all variables in the symbol table
std::vector<std::string> Memory::getVariableNames() const {
    std::vector<std::string> names;
    for (const auto& pair : symbolTable_) {
        names.push_back(pair.first);
    }
    return names;
}

