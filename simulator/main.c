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
    } else if (instruction == instructions.nop) {     //
    } else if (instruction == instructions.rrmovl) {  // [src / dst]
      store.registers[nr[1]] = store.registers[nr[0]];
      i++;
    } else if (instruction == instructions.cmovle) {
    } else if (instruction == instructions.cmovl) {
    } else if (instruction == instructions.cmove) {
    } else if (instruction == instructions.cmovne) {
    } else if (instruction == instructions.cmovge) {
    } else if (instruction == instructions.cmovg) {
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

      m(store.registers[dst] + offset, store.registers[src]);
    } else if (instruction ==
               instructions.mrmovl) {  // [src / dst] [[[[offset]]]]
      byte src = nr[0];
      byte dst = nr[1];
      i++;

      word offset = nw;
      i += 4;

      store.registers[dst] = mr(store.registers[src] + offset);
    } else if (instruction == instructions.addl) {  // [src / dst]
      store.registers[nr[1]] += store.registers[nr[0]];
      i++;
    } else if (instruction == instructions.subl) {  // [src / dst]
      store.registers[nr[1]] -= store.registers[nr[0]];
      i++;
    } else if (instruction == instructions.andl) {  // [src / dst]
      store.registers[nr[1]] &= store.registers[nr[0]];
      i++;
    } else if (instruction == instructions.xorl) {  // [src / dst]
      store.registers[nr[1]] ^= store.registers[nr[0]];
      i++;
    } else if (instruction == instructions.jmp) {  // [[[[dst]]]]
    } else if (instruction == instructions.jle) {  // [[[[dst]]]]
    } else if (instruction == instructions.jl) {   // [[[[dst]]]]
    } else if (instruction == instructions.je) {   // [[[[dst]]]]
    } else if (instruction == instructions.jne) {  // [[[[dst]]]]
    } else if (instruction == instructions.jge) {  // [[[[dst]]]]
    } else if (instruction == instructions.jg) {   // [[[[dst]]]]
    } else if (instruction == instructions.call) {
    } else if (instruction == instructions.ret) {
    } else if (instruction == instructions.pushl) {  // [src / f]
      word address = (store.registers[registers.esp] -= 4);
      m(address, store.registers[nr[0]]);

      i++;
    } else if (instruction == instructions.popl) {  // [src / f]
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
