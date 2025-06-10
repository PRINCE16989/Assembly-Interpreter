#pragma once
#include <array>
#include <memory>
#include <string>
#include "Memory.h"
#include "Instruction.h"

class CPU {
public:
    explicit CPU(const std::string& instructionFile, const std::string& dataFile);

    // Run until halt or program ends
    void run();

    // Run a single instruction, returns false if program ends 
    bool step();

    // Register access for executing instructions
    // Returns the value of the register at index idx (0-31)
    // Throws std::out_of_range if idx is invalid
    int32_t getRegister(size_t idx) const;
    void setRegister(size_t idx, int32_t value);

    // PC accessor
    uint32_t getPC() const;
    void setPC(uint32_t pc);

    // Print registers (for debug)
    void printRegisters() const;
    Memory* getMemory() const { return memory_; }

private:
    // Main instruction dispatcher
    Memory* memory_; // Memory containing instructions and data
    void execute(const Instruction& inst);
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
    // ... (other opcodes)

    std::array<int32_t, 32> registers_; // RISC-V: 32 registers
    uint32_t pc_; // Program counter
};