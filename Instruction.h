#pragma once
#include <string>
#include <vector>

enum class Opcode {
    NOP,
    SW, SH, SB, LH, LB, LHU, LBU, LW, LI, 
    LUI, AUIPC, JAL, JALR, LA,
    ADD, SUB, AND, OR, XOR, MUL,
    SLL, SRL, SRA,
    ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,
    BEQ, BNE, BLT, BGE, BLTU, BGEU,
    INVALID
};

struct Operands {
    std::string rd;    // destination register
    std::string rs1;   // source register
    std::string rs2;   // second source register (if applicable)
    std::string var;   // variable name (if applicable)
    int immediate;     // immediate value
    bool valid;        // successfully parsed?
};

class Instruction {
private:
    Opcode opcode;
    Operands operands;
    bool valid;
    std::string originalLine;

    // Helper function to trim whitespace from a string
    std::string trim(const std::string& s);

    // Parsing functions for different operand formats
    Operands parseTwoRegOneImm(const std::string& operandPart);
    Operands parseThreeReg(const std::string& operandPart);
    Operands parseRegImm(const std::string& operandPart);
    Operands parseStoreLoad(const std::string& operandPart);
    Operands parseLoadAddress(const std::string& operandPart);
public:
    // constructor which decodes the instruction from a line of text
    Instruction(const std::string& line);  
    
    // Getters
    Opcode getOpcode() const { return opcode; }
    const Operands& getOperands() const { return operands; }
    bool isValid() const { return valid; }
    
    // Get the original line of the instruction
    std::string toString() const;

    // used for decoding the opcode from a string
    static Opcode stringToOpcode(const std::string&);

friend class CPU;
};

inline int32_t bit_12OverflowSim(int32_t x)
{
    //x is offset
    //z is overflow
    int32_t z=x;
    if (x > 2047)
    {
        z = x % 2047;
        z = -2048 + z;
    }
    else if (x < -2048)
    {
        z = x % 2048;
        z = 0 - z;
    }
    return z;
}
