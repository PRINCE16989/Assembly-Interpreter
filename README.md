# Assembly-Interpreter

This project implements a simple Assembly language interpreter in C++. It consists of several components that collectively simulate a basic CPU, memory, and instruction execution environment.

## Contents

- **CPU.cpp / CPU.h** : Implements the CPU logic, fetches the instruction as **Instruction** which contains the decoded instruction, executes the instruction.
- **Instruction.cpp / Instruction.h** : decodes the opcode and operand from the instruction(line).
- **Memory.cpp / Memory.h** : Represents the memory model used by the interpreter and contains logic for taking input(instructions && variables used in code,either from files or manually).
- **interpreter.cpp** : manages program execution, main entry point for the interpreter.
- **default_instruction.txt/ default_data** : name of the default files loaded into the program

## Some syntax Differences from RISC-V Assembly
- use **capital letters** for operations(e.g. ADD, SUB, LI, LW...)
- Do not insert **comments** in between instructions. The parsing algorithm is not designed to handle comments or empty lines.
- When using variable names in the code, ensure that the variables have been allotted space in the memory before simulating the code.
- Addition of variables using a file, the file should be done in the format given below, Adding variables in any different manner may lead to undefined behavior.
  ```
  variable_name  address_allotted_to_the variable   value_of_the_variable
  ```
   - Make sure that different addresses allotted have different values, if not then two different variables which are intended to contain two different values may end up pointing to same address and hence may have same value.
- The format for Instructions goes as follows :
    - Each instruction must have valid operation and operands (else will lead to undefined behavior or hopefully an error).
    - Changing the value of x0 is invalid which you can change as we only try to interpret simple assembly codes which are unlikely to use the fact that x0 must be 0.
    - You can use spaces as needed to maintain the neatness of the program (I'd suggest not to).
- In case of **Branch Instructions or Jump Instructions** in the assembly code, do not use any labels for the lines, instead use the line offset from the present line.

  **Code for finding nth fibonacci number** :
     ```
        1. LI x1, 0
        2. LI x2, 1
        3. LI x3, 2
        4. LA x5, n              // to find nth fibonacci number
        5. LW x4, 0(x5)
        6. ADDI x4, x4, 1
        7. LA x5, result         // Make sure result is allotted space in the memory
                                 // can use manual addition of variables if needed.
                                 // answer will be stored in result which will appear in data.txt after running the program.
        8. ADD x6, x1, x2
        9. ADD x1, x2, x0
        10.ADD x2, x6, x0
        11.ADDI x3, x3, 1
        12.SUB x7, x4, x3
        13.BNE x7, x0, -5        // here the execution if this instruction takes pc to line 8
                                 // i.e., to the instruction ADD x6, x1, x2 
        14.SW x6, 0(x5)
     ```
  - This code is just for reference. The comments must be removed before trying to execute this using the interpreter.
## Execution
Download all the files into a folder.
1. Compile the project:
  ```
  g++ -o interpretor interpreter.cpp CPU.cpp Memory.cpp Instruction.cpp
  ```
2. Run the interpreter with a input file: 
  ```
  ./interpreter < input.txt
  ```
- If you want the program to be interactive type remove the comments for promt messages from the code and execute it without an input file

Some Example assembly codes are given above(fibonacci, sum, gcd, reversing an array), along with some testcases for each of the code.
You can run those using the shell script provided.
