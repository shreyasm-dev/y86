#include "store.h"

#include "../common/includes.h"

struct store store = {.registers = NULL, .memory = NULL};

void init_store() {
  if (store.registers != NULL || store.memory != NULL) {
    return;
  }

  store.registers = (word*)malloc(8 * sizeof(word));
  for (int i = 0; i < 8; i++) {
    store.registers[i] = 0;
  }

  store.memory = (word*)malloc(0xf00 * sizeof(word));
  for (int i = 0; i < 0x100; i++) {
    store.memory[i] = 0;
  }
}
