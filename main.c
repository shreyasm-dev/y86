#define _ source[i]
#define __ source[i + 1]

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instructions.h"
#include "registers.h"
#include "util.h"

int i;
int address;
uint8_t* result;

void pushb(uint8_t value) {
  result = (uint8_t*)realloc(result, (address + 1) * sizeof(uint8_t));
  result[address] = value;
  address++;
}

void pushi(int value) {
  pushb((uint8_t)((value >> 8) & 0xff));
  pushb((uint8_t)(value & 0xff));
}

// TODO: 0x- prefix
int main(int argc, char** argv) {
  char** source = (char**)malloc(3 * sizeof(char*));
  source[0] = "irmovl";
  source[1] = "\%eax";
  source[2] = "6251";

  i = 0;
  address = 0;
  result = (uint8_t*)malloc(0);

  while (_ != NULL) {
    if (eq(_, "halt")) { // halt -> halt
      pushb(instructions.halt); // halt
      i++;
    } else if (eq(_, "nop")) { // nop -> nop
      pushb(instructions.nop); // nop
      i++;
    } else if (eq(_, "rrmovl")) { // rrmovl a, b -> rrmovl a b
      pushb(instructions.rrmovl); // rrmovl
      i++;

      pushb((r(_) << 4) | r(__)); // a, b
      i += 2;
    } else if (eq(_, "irmovl")) { // irmovl v, b -> irmovl f/b v
      pushb(instructions.irmovl); // irmovl
      i++;

      byte b = 0xf0 | r(_);
      i++;

      pushi(atoi(_)); // v
      i++;

      pushb(b); // f/b
    } else if (eq(_, "ret")) { // ret -> ret
      pushb(instructions.ret); // ret
      i++;
    } else {
      printf("Unknown instruction: %s\n", _);
      return 1;
    }
  }

  for (int i = 0; i < address; i++) {
    printf("%02x\n", result[i]);
  }
}
