#include "Instruction.h"
#include <sstream>
#include <algorithm>
#include <unordered_map>

static const std::unordered_map<std::string, Opcode> table = {
    {"NOP", Opcode::NOP},
    {"SW", Opcode::SW}, {"SH", Opcode::SH}, {"SB", Opcode::SB}, {"LH", Opcode::LH}, {"LB", Opcode::LB},
    {"LHU", Opcode::LHU}, {"LBU", Opcode::LBU}, {"LW", Opcode::LW}, {"LI", Opcode::LI},
    {"LUI", Opcode::LUI}, {"AUIPC", Opcode::AUIPC}, {"JAL", Opcode::JAL}, {"JALR", Opcode::JALR},
    {"LA", Opcode::LA},
    {"ADD", Opcode::ADD}, {"SUB", Opcode::SUB}, {"AND", Opcode::AND}, {"OR", Opcode::OR},
    {"XOR", Opcode::XOR}, {"SLL", Opcode::SLL}, {"SRL", Opcode::SRL}, {"SRA", Opcode::SRA}, {"ADDI", Opcode::ADDI},
    {"SLTI", Opcode::SLTI}, {"SLTIU", Opcode::SLTIU}, {"XORI", Opcode::XORI}, {"ORI", Opcode::ORI},
    {"ANDI", Opcode::ANDI}, {"SLLI", Opcode::SLLI}, {"SRLI", Opcode::SRLI}, {"SRAI", Opcode::SRAI},
    {"BEQ", Opcode::BEQ}, {"BNE", Opcode::BNE}, {"BLT", Opcode::BLT}, {"BGE", Opcode::BGE},
    {"BLTU", Opcode::BLTU}, {"BGEU", Opcode::BGEU},
    {"INVALID", Opcode::INVALID}

};

Opcode Instruction::stringToOpcode(const std::string& s) {    
    auto it = table.find(s);
    return it != table.end() ? it->second : Opcode::INVALID;
}

Instruction::Instruction(const std::string& line) : originalLine(line), valid(false) {
    std::istringstream iss(line);
    std::string op;
    iss >> op;
    opcode = stringToOpcode(op);
    if (opcode == Opcode::INVALID) return;

    std::string operandsPart = line.substr(op.length());
    if (opcode == Opcode::ADDI || opcode == Opcode::SLTI || opcode == Opcode::SLTIU || opcode == Opcode::XORI || opcode == Opcode::ORI
        || opcode == Opcode::ANDI || opcode == Opcode::SLLI || opcode == Opcode::SRLI || opcode == Opcode::SRAI || opcode == Opcode::JALR
        || opcode == Opcode::BEQ || opcode == Opcode::BNE || opcode == Opcode::BLT || opcode == Opcode::BGE || opcode == Opcode::BLTU 
        || opcode == Opcode::BGEU) {
            operands = parseTwoRegOneImm(operandsPart);
            valid = operands.valid;
        }
    else if(opcode == Opcode::ADD || opcode == Opcode::SUB || opcode == Opcode::AND || opcode == Opcode::OR || opcode == Opcode::XOR
            || opcode == Opcode::SLL || opcode == Opcode::SRL || opcode == Opcode::SRA) {
                operands = parseThreeReg(operandsPart);
                valid = operands.valid;
            }
    else if(opcode == Opcode::LUI || opcode == Opcode::AUIPC || opcode == Opcode::JAL || opcode == Opcode::LI) {
                operands = parseRegImm(operandsPart);
                valid = operands.valid;
    }
    else if(opcode == Opcode::SW || opcode == Opcode::SH || opcode == Opcode::SB || opcode == Opcode::LH || opcode == Opcode::LB
            || opcode == Opcode::LHU || opcode == Opcode::LBU || opcode == Opcode::LW) {
                operands = parseStoreLoad(operandsPart);
                valid = operands.valid;
    }
    else if(opcode == Opcode::LA) {
                operands = parseLoadAddress(operandsPart);
                valid = operands.valid;
    }
    else if(opcode == Opcode::NOP) {
        valid = true; // NOP is always valid
    }
    if(!valid) {
        throw std::runtime_error("Invalid operands in instruction: " + line);
    }
}

std::string Instruction::toString() const {
    return originalLine;
}