#include "spimcore.h"
#include <stdlib.h>
#include <stdio.h>

/* ALU */
/* 10 Points */
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero)
{
    printf("A: %x\nB: %x\nALUControl: %s\nALUresult: %x\nZero: %c\n\n", A, B, &ALUControl, *ALUresult, *Zero);
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
    printf("PC: %x\nMem: %x\nInstruction: %x\n\n", PC, *Mem, *instruction);
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    printf("instruction: %x\nop: %x\nr1: %x\nr2: %x\nr3 %x\nfunct: %x\noffset: %x\njsec: %x\n\n", instruction, *op, *r1, *r2, *r3, *funct, *offset, *jsec);
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls)
{
    printf("op: %x\n struct_controls: %p\n\n", op, controls);
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2)
{
    printf("r1: %x\nr2: %x\nReg: %x\ndata1: %x\ndata2: %x\n\n", r1, r2, *Reg, *data1, *data2);
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset, unsigned *extended_value)
{
    printf("offset: %x\nextended_value: %x\n\n", offset, *extended_value);
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero)
{
    printf("data1: %x\ndata2: %x\nextended_value: %x\nfunct: %x\nALUop: %c\nALUSrc: %c\nALUresult: %x\nZero: %c\n\n", data1, data2, extended_value, funct, ALUOp, ALUSrc, *ALUresult, *Zero);
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem)
{
    printf("ALUresult: %x\ndata2: %x\nMemWrite: %c\nMemRead: %c\nmemdata: %x\nMem: %x\n\n", ALUresult, data2, MemWrite, MemRead, *memdata, *Mem);
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg)
{
    printf("r2: %x\nr3: %x\nmemdata: %x\nALUresult: %x\nRegWrite: %c\nRegDst: %c\nMemtoReg: %c\nReg: %x\n\n", r2, r3, memdata, ALUresult, RegWrite, RegDst, MemtoReg, *Reg);
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
    printf("jsec: %x\nextended_value: %x\nBranch: %c\nJump: %c\nZero: %c\nPC: %x\n\n", jsec, extended_value, Branch, Jump, Zero, *PC);
}
