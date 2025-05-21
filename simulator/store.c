#include "store.h"

#include "../common/includes.h"

long MEMORY_SIZE = 0xf00;

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

  store.memory = (word*)malloc(MEMORY_SIZE * sizeof(word));
  for (int i = 0; i < MEMORY_SIZE; i++) {
    store.memory[i] = 0;
  }
}

void ms(word address, word data) {
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
