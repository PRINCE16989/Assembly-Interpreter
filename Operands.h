#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

struct Operands {
    std::string rd;    // destination register
    std::string rs1;   // source register
    std::string rs2;   // second source register (if applicable)
    std::string var;   // variable name (if applicable)
    int immediate;     // immediate value
    bool valid;        // successfully parsed?
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

// Trims whitespace from both ends
inline std::string trim(const std::string& s) {
    size_t first = s.find_first_not_of(" \t");
    if (first == std::string::npos) return "";
    size_t last = s.find_last_not_of(" \t");
    return s.substr(first, last - first + 1);
}

// Parses a line like "x1, x2, 5" or "x10,x11,-8"
inline Operands parseTwoRegOneImm(const std::string& operandPart) {
    Operands result;
    std::stringstream ss(operandPart);
    std::string rd, rs1, imm;
    // Comma-separated
    if (!std::getline(ss, rd, ',')) return { "", "", "", "", 0, false };
    if (!std::getline(ss, rs1, ',')) return { "", "", "", "", 0, false };
    if (!std::getline(ss, imm))      return { "", "", "", "", 0, false };

    result.rd  = trim(rd);
    result.rs1 = trim(rs1);
    // Trim and parse immediate
    imm = trim(imm);
    try {
        result.immediate = bit_12OverflowSim(std::stoi(imm));
        result.valid = true;
    } catch (...) {
        result.valid = false;
    }
    return result;
}

inline Operands parseThreeReg(const std::string& operandPart) {
    Operands result;
    std::stringstream ss(operandPart);
    std::string rd, rs1, rs2;
    // Comma-separated
    if (!std::getline(ss, rd, ',')) return { "", "", "", "", 0, false };
    if (!std::getline(ss, rs1, ',')) return { "", "", "", "", 0, false };
    if (!std::getline(ss, rs2))       return { "", "", "", "", 0, false };

    result.rd  = trim(rd);
    result.rs1 = trim(rs1);
    result.rs2 = trim(rs2);
    result.valid = true;
    return result;
}

inline Operands parseRegImm(const std::string& operandPart) {
    Operands result;
    std::stringstream ss(operandPart);
    std::string rd, imm;
    // Comma-separated
    if (!std::getline(ss, rd, ',')) return { "", "", "", "", 0, false };
    if (!std::getline(ss, imm))      return { "", "", "", "", 0, false };

    result.rd = trim(rd);
    // Trim and parse immediate
    imm = trim(imm);
    try {
        result.immediate = bit_12OverflowSim(std::stoi(imm));
        result.valid = true;
    } catch (...) {
        result.valid = false;
    }
    return result;
}

inline Operands parseStoreLoad(const std::string& operandPart) {
    Operands result;
    std::stringstream ss(operandPart);
    std::string rd, rs1, imm;
    // Comma-separated
    if (!std::getline(ss, rd, ',')) return { "", "", "", "", 0, false };
    if (!std::getline(ss, imm, '(')) return { "", "", "", "", 0, false };
    if (!std::getline(ss, rs1, ')')) return { "", "", "", "", 0, false };

    result.rd  = trim(rd);
    result.rs1 = trim(rs1);
    // Trim and parse immediate
    imm = trim(imm);
    try {
        result.immediate = bit_12OverflowSim(std::stoi(imm));
        result.valid = true;
    } catch (...) {
        result.valid = false;
    }
    return result;
}

inline Operands parseLoadAddress(const std::string& operandPart) {
    Operands result;
    std::stringstream ss(operandPart);
    std::string rd, var;
    // Comma-separated
    if (!std::getline(ss, rd, ',')) return { "", "", "", "", 0, false };
    if (!std::getline(ss, var))      return { "", "", "", "", 0, false };

    result.rd  = trim(rd);
    result.var = trim(var);
    result.valid = true;
    return result;
}