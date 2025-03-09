#include "includes.h"
#include "util.h"

struct instructions {
  byte halt;
  byte nop;

  byte rrmovl;
  byte cmovle;
  byte cmovl;
  byte cmove;
  byte cmovne;
  byte cmovge;
  byte cmovg;

  byte irmovl;
  byte rmmovl;
  byte mrmovl;

  byte addl;
  byte subl;
  byte andl;
  byte xorl;

  byte jmp;
  byte jle;
  byte jl;
  byte je;
  byte jne;
  byte jge;
  byte jg;

  byte call;
  byte ret;
  byte pushl;
  byte popl;
};

extern const struct instructions instructions;
