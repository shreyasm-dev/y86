#include "util.h"

#include "includes.h"

struct array_map {
  char** keys;
  int* values;
  int n;
};

struct array_map lookup = {
    .keys = NULL,
    .values = NULL,
    .n = 0,
};

void lookup_init() {
  lookup.keys = (char**)malloc(0);
  lookup.values = (int*)malloc(0);
  lookup.n = 0;
}

int lookup_get(char* key) {
  for (int i = 0; i < lookup.n; i++) {
    if (eq(lookup.keys[i], key)) {
      return lookup.values[i];
    }
  }

  return -1;
}

int lookup_set(char* key, int value) {
  for (int i = 0; i < lookup.n; i++) {
    if (eq(lookup.keys[i], key)) {
      lookup.values[i] = value;
      return 0;
    }
  }

  lookup.keys = (char**)realloc(lookup.keys, (lookup.n + 1) * sizeof(char*));
  lookup.values = (int*)realloc(lookup.values, (lookup.n + 1) * sizeof(int));

  lookup.keys[lookup.n] = key;
  lookup.values[lookup.n] = value;
  lookup.n++;

  return 0;
}

bool eq(char* a, char* b) { return !strcmp(a, b); }

char* read_string(char* filename) {
  FILE* file = fopen(filename, "r");

  char* str = (char*)malloc(0);
  char c;

  while ((c = fgetc(file)) != EOF) {
    str = (char*)realloc(str, (strlen(str) + 1) * sizeof(char));
    str[strlen(str)] = c;
  }

  fclose(file);

  return str;
}
