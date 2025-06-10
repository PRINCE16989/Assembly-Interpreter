#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include "parsedOperands.h"

enum class Opcode {
    NOP,
    SW, SH, SB, LH, LB, LHU, LBU, LW, LI, 
    LUI, AUIPC, JAL, JALR, LA,
    ADD, SUB, AND, OR, XOR,
    SLL, SRL, SRA,
    ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,
    BEQ, BNE, BLT, BGE, BLTU, BGEU,
    INVALID
};

class Instruction {
private:
    Opcode opcode;
    Operands operands;
    bool valid;
    std::string originalLine;
public:
    Instruction(const std::string& line);

    Opcode getOpcode() const { return opcode; }
    const Operands& getOperands() const { return operands; }
    bool isValid() const { return valid; }
    std::string toString() const;
    static Opcode stringToOpcode(const std::string&);
friend class CPU;
};