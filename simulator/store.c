#include "store.h"

#include "../common/includes.h"

long MEMORY_SIZE = 0xfff;

struct store store = {
    .registers = NULL, .memory = NULL, .zero = 0, .sign = 0, .overflow = 0};

void init_store() {
  if (store.registers != NULL || store.memory != NULL) {
    return;
  }

  store.registers = (word*)malloc(8 * sizeof(word));
  for (int i = 0; i < 8; i++) {
    store.registers[i] = 0;
  }

  store.registers[registers.esp] = MEMORY_SIZE;

  store.memory = (byte*)malloc(MEMORY_SIZE * sizeof(byte));
  for (int i = 0; i < MEMORY_SIZE; i++) {
    store.memory[i] = 0;
  }
}

void mw(word address, word data) {
  store.memory[address] = data & 0xff;
  store.memory[address + 1] = (data & 0xff00) >> 8;
  store.memory[address + 2] = (data & 0xff0000) >> 16;
  store.memory[address + 3] = (data & 0xff000000) >> 24;
}

word mr(word address) {
  return (store.memory[address] | (store.memory[address + 1] << 8) |
          (store.memory[address + 2] << 16) |
          (store.memory[address + 3] << 24));
}

void print_store() {
  char* register_list[] = {
      "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi",
  };
  char** register_values = malloc(8 * sizeof(char*));

  for (int i = 0; i < 8; i++) {
    register_values[i] = malloc(10 * sizeof(char));
    sprintf(register_values[i], "%u", store.registers[i]);
  }

  table(
      (char**[]){
          register_list,
          register_values,
      },
      2, 8, false);

  printf("\n");

  table(
      (char**[]){
          (char*[]){"zero", store.zero ? "true" : "false"},
          (char*[]){"sign", store.sign ? "true" : "false"},
          (char*[]){"overflow", store.overflow ? "true" : "false"},
      },
      3, 2, true);

  printf("\n");
  for (int i = MEMORY_SIZE; i > store.registers[registers.esp]; i--) {
    printf("%02x ", store.memory[i - 1]);
    if (i % 4 == 0) {
      printf("\n");
    }
  }
}
