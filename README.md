# Assembly-Interpreter

This project implements a simple Assembly language interpreter in C++. It consists of several components that collectively simulate a basic CPU, memory, and instruction execution environment.

__contents__

**CPU.cpp /CPU.h** : Implements the CPU logic, fetches the instruction as **Instruction** which contains the decoded instruction, executes the instruction
**Instruction.cpp / Instruction.h** : decodes the opcode from the instruction(line) and calls for the right parsing function from **Operands.h** 
