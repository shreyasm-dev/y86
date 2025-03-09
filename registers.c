#include "registers.h"

#include "includes.h"
#include "util.h"

const struct registers registers = {
    .eax = 0x0,
    .ecx = 0x1,
    .edx = 0x2,
    .ebx = 0x3,
    .esp = 0x4,
    .ebp = 0x5,
    .esi = 0x6,
    .edi = 0x7,
};

byte r(char* name) {
  if (eq(name, "%eax")) {
    return registers.eax;
  } else if (eq(name, "%ecx")) {
    return registers.ecx;
  } else if (eq(name, "%edx")) {
    return registers.edx;
  } else if (eq(name, "%ebx")) {
    return registers.ebx;
  } else if (eq(name, "%esp")) {
    return registers.esp;
  } else if (eq(name, "%ebp")) {
    return registers.ebp;
  } else if (eq(name, "%esi")) {
    return registers.esi;
  } else if (eq(name, "%edi")) {
    return registers.edi;
  } else {
    printf("unknown register: %s\n", name);
    exit(1);
  }
}
