#include "instructions.h"

#include "includes.h"

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

    .call = 0x80,
    .ret = 0x90,
    .pushl = 0xa0,
    .popl = 0xb0,
};

struct array_map instruction_lookup;

void init_instruction_lookup() {
  if (instruction_lookup.n > 0) {
    return;
  }

  instruction_lookup = create_map();
  set(&instruction_lookup, "halt", instructions.halt);
  set(&instruction_lookup, "nop", instructions.nop);

  set(&instruction_lookup, "rrmovl", instructions.rrmovl);
  set(&instruction_lookup, "cmovle", instructions.cmovle);
  set(&instruction_lookup, "cmovl", instructions.cmovl);
  set(&instruction_lookup, "cmove", instructions.cmove);
  set(&instruction_lookup, "cmovne", instructions.cmovne);
  set(&instruction_lookup, "cmovge", instructions.cmovge);
  set(&instruction_lookup, "cmovg", instructions.cmovg);

  set(&instruction_lookup, "irmovl", instructions.irmovl);
  set(&instruction_lookup, "rmmovl", instructions.rmmovl);
  set(&instruction_lookup, "mrmovl", instructions.mrmovl);

  set(&instruction_lookup, "addl", instructions.addl);
  set(&instruction_lookup, "subl", instructions.subl);
  set(&instruction_lookup, "andl", instructions.andl);
  set(&instruction_lookup, "xorl", instructions.xorl);

  set(&instruction_lookup, "jmp", instructions.jmp);
  set(&instruction_lookup, "jle", instructions.jle);
  set(&instruction_lookup, "jl", instructions.jl);
  set(&instruction_lookup, "je", instructions.je);
  set(&instruction_lookup, "jne", instructions.jne);
  set(&instruction_lookup, "jge", instructions.jge);
  set(&instruction_lookup, "jg", instructions.jg);

  set(&instruction_lookup, "call", instructions.call);
  set(&instruction_lookup, "ret", instructions.ret);
  set(&instruction_lookup, "pushl", instructions.pushl);
  set(&instruction_lookup, "popl", instructions.popl);
}

byte s(char* name) {
  init_instruction_lookup();
  return get(instruction_lookup, name);
}
