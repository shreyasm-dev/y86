#include "../common/includes.h"
#include "store.h"

int main(int argc, char** argv) {
  bool print_steps = true;

  init_store();

  char* filename =
      (char*)malloc(strlen(argv[1]) + strlen("/test/") + strlen(argv[2]) + 1);
  strcat(filename, argv[1]);
  strcat(filename, "/test/");
  strcat(filename, argv[2]);

  long len = 0;
  byte* source = read_ascii_hex(filename, &len);

  for (int i = 0; i < len; i++) {
    store.memory[i] = source[i];
  }

  free(source);
  source = store.memory;

  long i = 0;

  while (true) {
    word instruction = source[i++];

    if (instruction == instructions.halt) {  //
      break;
    } else if (instruction == instructions.nop) {  //
    } else if (_$$(rrmovl)) {                      // [src / dst]
      bool move = false;

      if (instruction == instructions.rrmovl) {
        move = true;
      } else if (instruction == instructions.cmovle) {
        move = (store.sign ^ store.overflow) | store.zero;
      } else if (instruction == instructions.cmovl) {
        move = store.sign ^ store.overflow;
      } else if (instruction == instructions.cmove) {
        move = store.zero;
      } else if (instruction == instructions.cmovne) {
        move = !store.zero;
      } else if (instruction == instructions.cmovge) {
        move = !(store.sign ^ store.overflow);
      } else if (instruction == instructions.cmovg) {
        move = !((store.sign ^ store.overflow) | store.zero);
      } else {
        unreachable();
      }

      if (move) {
        store.registers[nr[1]] = store.registers[nr[0]];
      }

      i++;
    } else if (instruction == instructions.irmovl) {  // [f / dst] [[[[val]]]]
      byte dst = nr[1];
      i++;

      store.registers[dst] = nw;
      i += 4;
    } else if (instruction ==
               instructions.rmmovl) {  // [src / dst] [[[[offset]]]]
      byte src = nr[0];
      byte dst = nr[1];
      i++;

      word offset = nw;
      i += 4;

      mw(store.registers[dst] + offset, store.registers[src]);
    } else if (instruction ==
               instructions.mrmovl) {  // [src / dst] [[[[offset]]]]
      byte src = nr[0];
      byte dst = nr[1];
      i++;

      word offset = nw;
      i += 4;

      store.registers[dst] = mr(store.registers[src] + offset);
    } else if (_$$(addl)) {  // [src / dst]
      word src = store.registers[nr[0]];
      word* dst = &store.registers[nr[1]];

      if (instruction == instructions.addl) {
        *dst += src;
      } else if (instruction == instructions.subl) {
        *dst -= src;
      } else if (instruction == instructions.andl) {
        *dst &= src;
      } else if (instruction == instructions.xorl) {
        *dst ^= src;
      } else {
        unreachable();
      }

      store.zero = (*dst == 0);
      store.sign = neg(*dst);
      store.overflow = (cmp(*dst, src) < 0) && (cmp(*dst, *dst - src) > 0);

      i++;
    } else if (_$$(jmp)) {  // [[[[dst]]]]
      bool jump = false;

      if (instruction == instructions.jmp) {
        jump = true;
      } else if (instruction == instructions.jle) {
        jump = (store.sign ^ store.overflow) | store.zero;
      } else if (instruction == instructions.jl) {
        jump = store.sign ^ store.overflow;
      } else if (instruction == instructions.je) {
        jump = store.zero;
      } else if (instruction == instructions.jne) {
        jump = !store.zero;
      } else if (instruction == instructions.jge) {
        jump = !(store.sign ^ store.overflow);
      } else if (instruction == instructions.jg) {
        jump = !((store.sign ^ store.overflow) | store.zero);
      } else {
        unreachable();
      }

      i = jump ? nw : i + 4;
    } else if (instruction == instructions.call) {  // [[[[dst]]]]
      word address = (store.registers[registers.esp] -= 4);
      mw(address, i + 4);
      i = nw;
    } else if (instruction == instructions.ret) {  //
      i = mr(store.registers[registers.esp]);
      store.registers[registers.esp] += 4;
    } else if (instruction == instructions.pushl) {  // [src / f]
      word address = (store.registers[registers.esp] -= 4);
      mw(address, store.registers[nr[0]]);

      i++;
    } else if (instruction == instructions.popl) {  // [src / f]
      word address = store.registers[registers.esp];
      store.registers[registers.esp] += 4;
      store.registers[nr[0]] = mr(address);

      i++;
    } else {
      error("unknown instruction code %x", instruction);
    }

    if (print_steps) {
      printf("\n");
      printf("instruction: %02x\n\n", instruction);
      print_store();
      printf("program counter: %ld\n", i);
      printf("\n==============================\n");
    }

    if (i >= MEMORY_SIZE) {
      break;
    }
  }

  if (!print_steps) {
    print_store();
  }
}
