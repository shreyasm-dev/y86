#include "../common/includes.h"

#ifndef STORE_H
#define STORE_H

long MEMORY_SIZE;
struct store {
  word* registers;
  byte* memory;
  bool zero;
  bool sign;
  bool overflow;
};

extern struct store store;

void init_store();

#endif
