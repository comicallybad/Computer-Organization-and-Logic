#include "spimcore.h"

/* ALU */
/* 10 Points */
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero)
{
    if (A - B == 0)
        *Zero = 1;
    else
        *Zero = 0;
    switch (ALUControl)
    {
    case 0x0:
        *ALUresult = A + B;
        break;
    case 0x1:
        *ALUresult = A - B;
        break;
    case 0x2:
        if (!(A & (1 << 31)) && !(B & (1 << 31))){
            if(A < B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
        }else if ((A & (1 << 31)) && (B & (1 << 31))){
            if(A > B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
        }else if (!(A & (1 << 31)) && (B & (1 << 31)))
            *ALUresult = 0;
        else
            *ALUresult = 1;
        break;
    case 0x3:
        if(A < B)
            *ALUresult = 1;
        else
            *ALUresult = 0;
        break;
    case 0x4:
        *ALUresult = A & B;
        break;
    case 0x5:
        *ALUresult = A | B;
        break;
    case 0x6:
        *ALUresult = B << 16;
        break;
    case 0x7:
        *ALUresult = ~A;
    }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
    if (PC % 4 == 0)
        *instruction = Mem[PC >> 2];
    else
        return 1;
    return 0;
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    *op = (instruction & 0xfc000000) >> 26;
    *r1 = (instruction & 0x03e00000) >> 21;
    *r2 = (instruction & 0x001f0000) >> 16;
    *r3 = (instruction & 0x0000f800) >> 11;
    *funct = instruction & 0x0000003f;
    *offset = instruction & 0x0000ffff;
    *jsec = instruction & 0x03ffffff;
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls)
{
    switch (op)
    {
    case 0x0:
        *controls = (struct_controls){1, 0, 0, 0, 0, 7, 0, 0, 1};
        break;
    case 0x8:
        *controls = (struct_controls){0, 0, 0, 0, 0, 0, 0, 1, 1};
        break;
    case 0x23:
        *controls = (struct_controls){0, 0, 0, 1, 1, 0, 0, 1, 1};
        break;
    case 0x2b:
        *controls = (struct_controls){0, 0, 0, 0, 0, 0, 1, 1, 0};
        break;
    case 0xf:
        *controls = (struct_controls){0, 0, 0, 0, 0, 6, 0, 1, 1};
        break;
    case 0x4:
        *controls = (struct_controls){2, 0, 1, 0, 2, 1, 0, 2, 0};
        break;
    case 0xa:
        *controls = (struct_controls){0, 0, 0, 0, 0, 2, 0, 1, 1};
        break;
    case 0xb:
        *controls = (struct_controls){0, 0, 0, 0, 0, 3, 0, 1, 1};
        break;
    case 0x2:
        *controls = (struct_controls){2, 1, 2, 0, 2, 0, 0, 2, 0};
        break;
    default:
        return 1;
    }
    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset, unsigned *extended_value)
{
    if ((offset >> 15))
        *extended_value = offset | 0xffff0000;
    else
        *extended_value = offset & 0x0000ffff;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero)
{
    unsigned char ALUControl = ALUOp;

    switch (ALUOp)
    {
    case 0x0:
    case 0x1:
    case 0x2:
    case 0x3:
    case 0x4:
    case 0x5:
    case 0x6:
        break;
    case 0x7:
        switch (funct)
        {
        case 0x20:
            ALUControl = 0x0;
            break;
        case 0x24:
            ALUControl = 0x4;
            break;
        case 0x25:
            ALUControl = 0x5;
            break;
        case 0x2a:
            ALUControl = 0x2;
            break;
        case 0x2b:
            ALUControl = 0x3;
            break;
        default:
            return 1;
        }
        break;
    default:
        return 1;
    }

    unsigned temp;
    if(ALUSrc == 1)
        temp = extended_value;
    else
        temp = data2;

    ALU(data1, temp, ALUControl, ALUresult, Zero);

    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem)
{
    if (MemRead)
    {
        if (ALUresult % 4 == 0 && ALUresult < 65536)
            *memdata = Mem[ALUresult >> 2];
        else
            return 1;
    }
    if (MemWrite)
    {
        if (ALUresult % 4 == 0 && ALUresult < 65536)
            Mem[ALUresult >> 2] = data2;
        else
            return 1;
    }
    return 0;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg)
{
    if (RegWrite)
    {
        if (MemtoReg)
            Reg[r2] = memdata;
        else if (!MemtoReg)
        {
            if (RegDst)
                Reg[r3] = ALUresult;
            else
                Reg[r2] = ALUresult;
        }
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
    *PC += 4;
    if (Zero && Branch)
        *PC = *PC + (extended_value << 2);
    if (Jump)
        *PC = (*PC & 0xf0000000) | (jsec << 2);
}