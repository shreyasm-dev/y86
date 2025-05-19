#include "../common/includes.h"

#ifndef STORE_H
#define STORE_H

struct store {
  word* registers;
  word* memory;
};

extern struct store store;

void init_store();

#endif
