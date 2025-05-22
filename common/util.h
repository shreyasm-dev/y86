#include "includes.h"

#ifndef UTIL_H
#define UTIL_H

#define push(to, x, n) { \
  to = realloc(to, (n + 1) * sizeof(x)); \
  (to)[(n)++] = x; \
}

#define success(...) { \
  printf("\033[0;32m"); \
  printf(__VA_ARGS__); \
  printf("\033[0m\n"); \
  exit(0); \
}

#define error(...) { \
  printf("\033[0;31m"); \
  printf(__VA_ARGS__); \
  printf("\033[0m\n"); \
  exit(1); \
}

#define bold(...) { \
  printf("\033[1m"); \
  printf(__VA_ARGS__); \
  printf("\033[0m"); \
}

#define unreachable() { \
  error("unreachable"); \
}

typedef uint8_t byte;
typedef uint32_t word;
#define MAX_WORD 4294967296;

typedef struct deferred_address {
  int address;
  char* label;
} deferred_address;

typedef struct array_map {
  char** keys;
  int* values;
  int n;
} array_map;

struct array_map create_map();
int get(struct array_map map, char* key);
int set(struct array_map* map, char* key, int value);

bool eq(char* a, char* b);
bool eq_any(char* a, char** b, int n);
bool ceq_any(char a, char* b);
char* read_string(char* filename);
byte* read_ascii_hex(char* filename, long* n);
bool neg(word n);
int cmp(word a, word b);
void table(char*** table, int m, int n, bool vertical);

#endif
