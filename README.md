![The P4 cpu](https://github.com/Pepe-57/p4-cpu/blob/main/p4.jpg)
# P4
P4 is a simple 4-bit single-cycle prosessor with 2-bit instruction set. Check the [user manual](https://github.com/Pepe-57/p4-cpu/blob/main/P4_user_manual_en.pdf) for more information

## Registers
The P4 contains two General-Purpose Registers (GPR) and two Special-Purpose Registers (SPR).
### GPR
- REG A: A 4-bit register that is used for the ALU.
- REG OR: A 4-bit register that is used for a result of an ALU operation.
### SPR
- PC: 4-bit or 6-bit register that points the next instruction in memory.
- IR: 6-bit register that is holds current instruction byte fetched from memory.
## Instruction set
The P4 has 4 diffent instructions. Every instruction takes a single clock cycle to complete.
- NOP    No operation
- LDA n  Load A with immediate value
- ADD n  Add A with immediate value -> OR
- JMP n  Unconditional jump, PC = immediate
