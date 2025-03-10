#include "includes.h"

#ifndef UTIL_H
#define UTIL_H

typedef uint8_t byte;

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

#endif
