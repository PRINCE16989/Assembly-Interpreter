#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include "Instruction.h" 

class Memory {
public:
    // Constructor: accepts filenames for code and data
    Memory(std::string& codeFile, std::string& dataFile);
    Memory() = default; // Default constructor for empty memory

    // Instruction memory
    void insertInstructionsManually(std::string& codeFile);
    void loadInstructionsFromFile(std::string& codeFile);
    void loadInstructionsUsingFile(std::string& filename);
    const Instruction& fetchInstruction(uint32_t pc) const;
    size_t instructionCount() const;

    // Data memory
    void loadVariablesFromFile(uint32_t& nextAddress, std::string& dataFile);
    void loadVariablesFromFile(const std::string& dataFile);
    void manualVariableInput(uint32_t& nextAddress, std::string& dataFile);
    void store(uint32_t address, int32_t value);
    int32_t load(uint32_t address) const;

    // Variable symbol table
    bool hasVariable(const std::string& name) const;
    uint32_t getVariableAddress(const std::string& name) const;
    std::vector<std::string> getVariableNames() const;
    void setVariable(const std::string& name, uint32_t address);
    
    // Save/restore data memory
    void clear();
    void saveDataToFile(const std::string& dataFile) const;

private:
    std::vector<Instruction> program_; // Loaded instructions
    std::unordered_map<uint32_t, int32_t> data_; // Address→value
    std::unordered_map<std::string, uint32_t> symbolTable_; // Variable→address
};
