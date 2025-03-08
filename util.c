#include "util.h"

const struct instructions instructions = {
    .halt = 0x00,
    .nop = 0x10,

    .rrmovl = 0x20,
    .cmovle = 0x21,
    .cmovl = 0x22,
    .cmove = 0x23,
    .cmovne = 0x24,
    .cmovge = 0x25,
    .cmovg = 0x26,

    .irmovl = 0x30,
    .rmmovl = 0x40,
    .mrmovl = 0x50,

    .addl = 0x60,
    .subl = 0x61,
    .andl = 0x62,
    .xorl = 0x63,

    .jmp = 0x70,
    .jle = 0x71,
    .jl = 0x72,
    .je = 0x73,
    .jne = 0x74,
    .jge = 0x75,
    .jg = 0x76,
};

const struct registers registers = {
    .eax = 0x00,
    .ecx = 0x01,
    .edx = 0x02,
    .ebx = 0x03,
    .esp = 0x04,
    .ebp = 0x05,
    .esi = 0x06,
    .edi = 0x07,
};
