#include "CPU.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>

CPU::CPU(std::string& instructionFile, std::string& dataFile) : registers_{}, pc_{0} {
        memory_ = new Memory(instructionFile, dataFile);
    }

CPU::CPU() : registers_{}, pc_{0} {
    memory_ = new Memory();
}

void CPU::run() {
    while (step());
}

// fetch, decode, execute cycle
bool CPU::step() {
    int32_t prevPC = pc_; // Save previous PC for debugging
    if (pc_ >= memory_->instructionCount())
        return false;

    const Instruction& inst = memory_->fetchInstruction(pc_);       // fetch and decode
    if(inst.getOpcode() == Opcode::NOP) return false;
    // std::cout << "\nExecuting instruction at PC " << pc_ << ": " << inst.toString() << std::endl;
    execute(inst);                                                  // execute the instruction
    registers_[0] = 0; // x0 is always zero, reset it after each instruction since JAL might modify it
    // print the state of registers for debugging
    // printRegisters();
    if(pc_ == prevPC) ++pc_; // May be changed by jump/branch
    return true;
}

void CPU::execute(const Instruction& inst) {
    switch (inst.getOpcode()) {
        case Opcode::ADD : executeADD(inst.getOperands());  break;
        case Opcode::ADDI: executeADDI(inst.getOperands()); break;
        case Opcode::SUB : executeSUB(inst.getOperands());  break;
        case Opcode::LI  : executeLI(inst.getOperands());   break;
        case Opcode::SW  : executeSW(inst.getOperands());   break;
        case Opcode::LA  : executeLA(inst.getOperands());   break;
        case Opcode::BEQ : executeBEQ(inst.getOperands());  break;
        case Opcode::BNE : executeBNE(inst.getOperands());  break;
        case Opcode::BGE : executeBGE(inst.getOperands());  break;
        case Opcode::BLT : executeBLT(inst.getOperands());  break;
        case Opcode::LW  : executeLW(inst.getOperands());   break;
        case Opcode::MUL : executeMUL(inst.getOperands());   break; 

        case Opcode::LUI   : executeLUI(inst.getOperands());       break;
        case Opcode::AUIPC : executeAUIPC(inst.getOperands(), pc_);break;
        case Opcode::AND   : executeAND(inst.getOperands());       break;
        case Opcode::OR    : executeOR(inst.getOperands());        break;
        case Opcode::XOR   : executeXOR(inst.getOperands());       break;
        case Opcode::ANDI  : executeANDI(inst.getOperands());      break;
        case Opcode::SLL   : executeSLL(inst.getOperands());       break;
        case Opcode::SRL   : executeSRL(inst.getOperands());       break;
        case Opcode::SRA   : executeSRA(inst.getOperands());       break;
        case Opcode::SLLI  : executeSLLI(inst.getOperands());      break; 
        case Opcode::SRLI  : executeSRL(inst.getOperands());       break;
        case Opcode::SRAI  : executeSRA(inst.getOperands());       break;
        case Opcode::JALR  : executeJALR(inst.getOperands(), pc_); break;
        case Opcode::JAL   : executeJAL(inst.getOperands(), pc_);  break;
                    
        default: throw std::runtime_error("Unknown opcode");
    }
}

// Getters and setters for registers
int32_t CPU::getRegister(size_t idx) const {
    if (idx >= registers_.size()) throw std::out_of_range("Register idx out of range");
    return registers_[idx];
}
void CPU::setRegister(size_t idx, int32_t value) {
    if (idx == 0) return; // x0 is always zero
    if (idx >= registers_.size()) throw std::out_of_range("Register idx out of range");
    registers_[idx] = value;
}
int32_t CPU::registerindex(std::string reg) {                 // working as intended
    if (reg[0] == 'x' && reg.length() > 1) {
        return stoi(reg.substr(1));
    }
    return -1; // Invalid register
}

// Getter for program counter (PC)
uint32_t CPU::getPC() const { return pc_; }

// Print the state of all registers
void CPU::printRegisters() const {
    for(int32_t i=0; i<32; ++i) {
        std::cout << "x" << std::left << std::setw(2) << i << ": " << std::left << std::setw(4) << registers_[i] << "  ";   
        if(i % 8 == 7) std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Logics for executing various instructions
void CPU::executeADD(const Operands& ops) {
    int32_t rdIdx = registerindex(ops.rd);
    int32_t rs1Idx = registerindex(ops.rs1);
    int32_t rs2Idx = registerindex(ops.rs2);
    if (rdIdx == -1 || rs1Idx == -1 || rs2Idx == -1) {
        throw std::runtime_error("Invalid operands for ADD");
    }
    if (rdIdx == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rdIdx, getRegister(rs1Idx) + getRegister(rs2Idx));
}

void CPU::executeSUB(const Operands& ops) {
    int32_t rdIdx = registerindex(ops.rd);
    int32_t rs1Idx = registerindex(ops.rs1);
    int32_t rs2Idx = registerindex(ops.rs2);
    if (rdIdx == -1 || rs1Idx == -1 || rs2Idx == -1) {
        throw std::runtime_error("Invalid operands for SUB");
    }
    if (rdIdx == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rdIdx, getRegister(rs1Idx) - getRegister(rs2Idx));
}

void CPU::executeADDI(const Operands& ops) {
    if(!ops.valid) throw std::runtime_error("Invalid operands for ADDI");
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    if (rd == -1 || rs1 == -1) {
        throw std::runtime_error("Invalid register in ADDI instruction");
    }
    int32_t imm = ops.immediate;
    if (rd == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rd, getRegister(rs1) + imm);
}

void CPU::executeLI(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    if (rd == -1) {
        throw std::runtime_error("Invalid register in LI instruction");
    }
    int32_t imm = ops.immediate;
    if (rd == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rd, imm);
}

void CPU::executeSW(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    if (rd == -1 || rs1 == -1) {
        throw std::runtime_error("Invalid register in SW instruction");
    }
    uint32_t address = ops.immediate + getRegister(rs1);
    memory_->store(address, getRegister(rd));
}

void CPU::executeLA(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    if (rd == -1) {
        throw std::runtime_error("Invalid register in LA instruction");
    }
    uint32_t address = memory_->getVariableAddress(ops.var);
    setRegister(rd, address);
}

void CPU::executeBEQ(const Operands& ops) {
    int32_t rs1 = registerindex(ops.rd);
    int32_t rs2 = registerindex(ops.rs1);
    if (rs1 == -1 || rs2 == -1) {
        throw std::runtime_error("Invalid register in BEQ instruction");
    }
    if (getRegister(rs1) == getRegister(rs2)) {
        pc_ += ops.immediate; // Branch taken
    }
}

void CPU::executeBNE(const Operands& ops) {
    int32_t rs1 = registerindex(ops.rd);
    int32_t rs2 = registerindex(ops.rs1);
    if (rs1 == -1 || rs2 == -1) {
        throw std::runtime_error("Invalid register in BNE instruction");
    }
    if (getRegister(rs1) != getRegister(rs2)) {
        pc_ += ops.immediate; // Branch taken
    }
}

void CPU::executeLW(const Operands& ops) {
    int32_t rs1 = registerindex(ops.rs1);
    if (rs1 == -1) {
        throw std::runtime_error("Invalid register in LW instruction");
    }
    uint32_t address = ops.immediate + getRegister(rs1);
    setRegister(registerindex(ops.rd), memory_->load(address));
}

void CPU::executeLUI(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    if (rd == -1) {
        throw std::runtime_error("Invalid register in LUI instruction");
    }
    setRegister(rd, ops.immediate << 12); // Load upper immediate
}

void CPU::executeAUIPC(const Operands& ops, uint32_t& PC) {
    int32_t rd = registerindex(ops.rd);
    if (rd == -1) {
        throw std::runtime_error("Invalid register in AUIPC instruction");
    }
    setRegister(rd, PC + (ops.immediate << 12)); // Add PC to immediate
}

void CPU::executeAND(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    int32_t rs2 = registerindex(ops.rs2);
    if (rd == -1 || rs1 == -1 || rs2 == -1) {
        throw std::runtime_error("Invalid operands for AND");
    }
    if (rd == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rd, getRegister(rs1) & getRegister(rs2));
}

void CPU::executeOR(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    int32_t rs2 = registerindex(ops.rs2);
    if (rd == -1 || rs1 == -1 || rs2 == -1) {
        throw std::runtime_error("Invalid operands for OR");
    }
    if (rd == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rd, getRegister(rs1) | getRegister(rs2));
}

void CPU::executeXOR(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    int32_t rs2 = registerindex(ops.rs2);
    if (rd == -1 || rs1 == -1 || rs2 == -1) {
        throw std::runtime_error("Invalid operands for XOR");
    }
    if (rd == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rd, getRegister(rs1) ^ getRegister(rs2));
}

void CPU::executeANDI(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    if (rd == -1 || rs1 == -1) {
        throw std::runtime_error("Invalid operands for ANDI");
    }
    if (rd == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rd, getRegister(rs1) & ops.immediate);
}

void CPU::executeSLL(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    int32_t rs2 = registerindex(ops.rs2);
    if (rd == -1 || rs1 == -1 || rs2 == -1) {
        throw std::runtime_error("Invalid operands for SLL");
    }
    if (rd == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rd, getRegister(rs1) << getRegister(rs2));
}

void CPU::executeSRL(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    int32_t rs2 = registerindex(ops.rs2);
    if (rd == -1 || rs1 == -1 || rs2 == -1) {
        throw std::runtime_error("Invalid operands for SRL");
    }
    if (rd == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rd, static_cast<uint32_t>(getRegister(rs1)) >> getRegister(rs2));
}

void CPU::executeSRA(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    int32_t rs2 = registerindex(ops.rs2);
    if (rd == -1 || rs1 == -1 || rs2 == -1) {
        throw std::runtime_error("Invalid operands for SRA");
    }
    if (rd == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rd, getRegister(rs1) >> getRegister(rs2));
}

void CPU::executeJALR(const Operands& ops, uint32_t& PC) {
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    if (rd == -1 || rs1 == -1) {
        throw std::runtime_error("Invalid operands for JALR");
    }
    setRegister(rd, PC + 4); // Save return address
    PC = (getRegister(rs1) + ops.immediate) & ~1; // Jump to target address
}

void CPU::executeJAL(const Operands& ops, uint32_t& PC) {
    int32_t rd = registerindex(ops.rd);
    if (rd == -1) {
        throw std::runtime_error("Invalid register in JAL instruction");
    }
    setRegister(rd, PC + 4); // Save return address
    PC += ops.immediate; // Jump to target address
}

void CPU::executeSRAI(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    if (rd == -1 || rs1 == -1) {
        throw std::runtime_error("Invalid operands for SRAI");
    }
    if (rd == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rd, getRegister(rs1) >> ops.immediate);
}

void CPU::executeSLLI(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    if (rd == -1 || rs1 == -1) {
        throw std::runtime_error("Invalid operands for SLLI");
    }
    if (rd == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rd, getRegister(rs1) << ops.immediate);
}

void CPU::executeSRLI(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    if (rd == -1 || rs1 == -1) {
        throw std::runtime_error("Invalid operands for SRLI");
    }
    if (rd == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rd, static_cast<uint32_t>(getRegister(rs1)) >> ops.immediate);
}

void CPU::executeBGE(const Operands& ops) {
    int32_t rs1 = registerindex(ops.rd);
    int32_t rs2 = registerindex(ops.rs1);
    if (rs1 == -1 || rs2 == -1) {
        throw std::runtime_error("Invalid register in BGE instruction");
    }
    if (getRegister(rs1) >= getRegister(rs2)) {
        pc_ += ops.immediate; // Branch taken
    }
}

void CPU::executeBLT(const Operands& ops) {
    int32_t rs1 = registerindex(ops.rd);
    int32_t rs2 = registerindex(ops.rs1);
    if (rs1 == -1 || rs2 == -1) {
        throw std::runtime_error("Invalid register in BLT instruction");
    }
    if (getRegister(rs1) < getRegister(rs2)) {
        pc_ += ops.immediate; // Branch taken
    }
}

void CPU::executeMUL(const Operands& ops) {
    int32_t rd = registerindex(ops.rd);
    int32_t rs1 = registerindex(ops.rs1);
    int32_t rs2 = registerindex(ops.rs2);
    if (rd == -1 || rs1 == -1 || rs2 == -1) {
        throw std::runtime_error("Invalid operands for MUL");
    }
    if (rd == 0) {
        throw std::runtime_error("Invalid Input! Can't modify register x0");
    }
    setRegister(rd, getRegister(rs1) * getRegister(rs2));
}
