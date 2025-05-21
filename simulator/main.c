#include "../common/includes.h"
#include "store.h"

int main(int argc, char** argv) {
  init_store();

  char* filename =
      (char*)malloc(strlen(argv[1]) + strlen("/test/") + strlen(argv[2]) + 1);
  strcat(filename, argv[1]);
  strcat(filename, "/test/");
  strcat(filename, argv[2]);

  long len = 0;
  byte* source = read_ascii_hex(filename, &len);

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

      store.overflow =
          !(negative(src) ^ negative(*dst));  // i.e. are the signs the same?
                                              // can't overflow if they're not

      if (instruction == instructions.addl) {
        *dst = src + *dst;
        store.overflow =
            store.overflow &&
            (negative(src) !=
             negative(*dst));  // if the sign changed, it overflowed
      } else if (instruction == instructions.subl) {
        *dst = src - *dst;
        store.overflow = store.overflow && (negative(src) != negative(*dst));
      } else if (instruction == instructions.andl) {
        *dst = src & *dst;
        store.overflow = 0;
      } else if (instruction == instructions.xorl) {
        *dst = src ^ *dst;
        store.overflow = 0;
      } else {
        unreachable();
      }

      store.sign = negative(*dst);
      store.zero = *dst == 0;

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

      i = jump ? mr(nw + 1) : i + 4;
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

    if (i >= MEMORY_SIZE) {
      break;
    }
  }

  printf("eax: %u\n", store.registers[registers.eax]);
  printf("ebx: %u\n", store.registers[registers.ebx]);
  printf("ecx: %u\n", store.registers[registers.ecx]);
}
