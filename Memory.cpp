#include "Memory.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
Memory::Memory(const std::string& codeFile, const std::string& dataFile) {
    // Load instructions from file
    loadInstructionsFromFile(codeFile);

    // Load data from file
    loadVariablesFromFile(dataFile);
}

void Memory::insertInstructionsManually() {
    std::cout << "Enter instructions (one per line, empty line to finish):\n";
    program_.clear(); // Clear existing instructions
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break; // Stop on empty line
        program_.emplace_back(line);
    }
}

void Memory::loadInstructionsFromFile() {
    std::cout << "Enter the instruction file name: ";
    std::string filename;
    std::cin >> filename;
    loadInstructionsFromFile(filename);
    
}

void Memory::loadInstructionsFromFile(const std::string& filename) {
    std::ifstream code(filename);
    if (!code) throw std::runtime_error("Failed to open instruction file.");
    std::string line;
    while (std::getline(code, line)) {
        if (line.empty()) continue; // Skip empty lines
        program_.emplace_back(line); // Assuming Instruction(line) parses each line
    }
    code.close();
    if (program_.empty()) {
        throw std::runtime_error("No valid instructions found in the file.");
    }
}

void Memory::loadVariablesFromFile(uint32_t& nextAddress) {
    std::cout << "Enter the data file name: ";
    std::string filename;
    std::cin >> filename;
    loadVariablesFromFile(filename);
    uint32_t maxAddr = 0;
    for (const auto& name : getVariableNames()) {
        uint32_t addr = getVariableAddress(name);
        if (addr > maxAddr) maxAddr = addr;
    }
    nextAddress = maxAddr + 4;
}

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

void Memory::manualVariableInput(uint32_t& nextAddress) {
    clear(); // Clear existing variables
    while(true) {
        std::cout << "Enter variable name (or 'exit' to stop): ";
        std::string varName;
        std::cin >> varName;
        if (varName == "exit") break;

        std::cout << "Enter variable value: ";
        int32_t value;
        std::cin >> value;

        setVariable(varName, nextAddress);
        store(nextAddress, value);
        nextAddress += 4; // Assume word-aligned, 4-byte variables
    }
}

const Instruction& Memory::fetchInstruction(uint32_t pc) const {
    if (pc >= program_.size()) throw std::out_of_range("PC out of range");
    return program_[pc];
}

void Memory::clear() { data_.clear(); symbolTable_.clear(); }

size_t Memory::instructionCount() const {
    return program_.size();
}

void Memory::store(uint32_t address, int32_t value) {
    data_[address] = value;
}

int32_t Memory::load(uint32_t address) const {
    auto it = data_.find(address);
    if (it == data_.end()) throw std::runtime_error("Address not initialized");
    return it->second;
}

bool Memory::hasVariable(const std::string& name) const {
    return symbolTable_.count(name) > 0;
}

uint32_t Memory::getVariableAddress(const std::string& name) const {
    auto it = symbolTable_.find(name);
    if (it == symbolTable_.end()) throw std::runtime_error("Variable not found");
    return it->second;
}

void Memory::setVariable(const std::string& name, uint32_t address) {
    symbolTable_[name] = address;
}

void Memory::saveDataToFile(const std::string& dataFile) const {
    std::ofstream out(dataFile);
    for (const auto& [name, addr] : symbolTable_) {
        out << name << " " << addr << " " << data_.at(addr) << "\n";
    }
}

std::vector<std::string> Memory::getVariableNames() const {
    std::vector<std::string> names;
    for (const auto& pair : symbolTable_) {
        names.push_back(pair.first);
    }
    return names;
}

