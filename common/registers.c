#include "includes.h"

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

struct array_map register_lookup;

void init_register_lookup() {
  if (register_lookup.n > 0) {
    return;
  }

  register_lookup = create_map();

  set(&register_lookup, "%eax", registers.eax);
  set(&register_lookup, "%ecx", registers.ecx);
  set(&register_lookup, "%edx", registers.edx);
  set(&register_lookup, "%ebx", registers.ebx);
  set(&register_lookup, "%esp", registers.esp);
  set(&register_lookup, "%ebp", registers.ebp);
  set(&register_lookup, "%esi", registers.esi);
  set(&register_lookup, "%edi", registers.edi);
}

byte r(char* name) {
  init_register_lookup();
  return get(register_lookup, name);
}
