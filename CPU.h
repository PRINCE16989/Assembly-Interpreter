#pragma once
#include <array>
#include <string>
#include "Memory.h"
#include "Instruction.h"

class CPU {
public:
    // Constructors
    CPU(std::string& instructionFile, std::string& dataFile);
    CPU();

    // To run the program
    void run();
    bool step();

    // Register access for executing instructions
    int32_t getRegister(size_t idx) const;
    void setRegister(size_t idx, int32_t value);

    // PC accessor
    uint32_t getPC() const;

    // Print registers (for debug)
    void printRegisters() const;

    // Get the memory object to store and retrieve data
    Memory* getMemory() const { return memory_; }

private:
    // Execute a single instruction
    void execute(const Instruction& inst);
    
    // Given a register name, return its index (0-31)
    int32_t registerindex(std::string reg);

    // Implementation for each opcode
    void executeADD(const Operands& ops);
    void executeADDI(const Operands& ops);
    void executeSUB(const Operands& ops);
    void executeLI(const Operands& ops);
    void executeSW(const Operands& ops);
    void executeLA(const Operands& ops);
    void executeBEQ(const Operands& ops);
    void executeBNE(const Operands& ops);
    void executeBGE(const Operands& ops);
    void executeBLT(const Operands& ops);
    void executeLW(const Operands& ops);
    void executeLUI(const Operands& ops);
    void executeAUIPC(const Operands& ops, uint32_t& PC);
    void executeAND(const Operands& ops);
    void executeOR(const Operands& ops);
    void executeXOR(const Operands& ops);
    void executeANDI(const Operands& ops);
    void executeSLL(const Operands& ops);
    void executeSRL(const Operands& ops);
    void executeSRA(const Operands& ops);
    void executeJALR(const Operands& ops, uint32_t& PC);
    void executeJAL(const Operands& ops, uint32_t& PC);
    void executeSLLI(const Operands& ops); 
    void executeSRLI(const Operands& ops); 
    void executeSRAI(const Operands& ops); 
    void executeMUL(const Operands& ops); 
    // ... (other opcodes)

    std::array<int32_t, 32> registers_; // RISC-V: 32 registers
    uint32_t pc_; // Program counter
    Memory* memory_; // Memory containing instructions and data
};
