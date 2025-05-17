#include "includes.h"

#ifndef REGISTERS_H
#define REGISTERS_H

struct registers {
  byte eax;
  byte ecx;
  byte edx;
  byte ebx;
  byte esp;
  byte ebp;
  byte esi;
  byte edi;
};

extern const struct registers registers;

byte r(char* name);

#endif
