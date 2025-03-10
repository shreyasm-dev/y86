#include "util.h"

#include "includes.h"

int get(struct array_map map, char* key) {
  for (int i = 0; i < map.n; i++) {
    if (eq(map.keys[i], key)) {
      return map.values[i];
    }
  }

  return -1;
}

int set(struct array_map* map, char* key, int value) {
  for (int i = 0; i < (*map).n; i++) {
    if (eq((*map).keys[i], key)) {
      (*map).values[i] = value;
      return 0;
    }
  }

  (*map).keys = (char**)realloc((*map).keys, ((*map).n + 1) * sizeof(char*));
  (*map).values = (int*)realloc((*map).values, ((*map).n + 1) * sizeof(int));

  (*map).keys[(*map).n] = key;
  (*map).values[(*map).n] = value;
  (*map).n++;

  return 0;
}

struct array_map create_map() {
  struct array_map m = {
      .keys = (char**)malloc(0),
      .values = (int*)malloc(0),
      .n = 0,
  };

  return m;
}

bool eq(char* a, char* b) { return !strcmp(a, b); }

bool eq_any(char* a, char** b, int n) {
  for (int i = 0; i < n; i++) {
    if (eq(a, b[i])) {
      return true;
    }
  }

  return false;
}

bool ceq_any(char a, char* b) {
  for (int i = 0; i < strlen(b); i++) {
    if (a == b[i]) {
      return true;
    }
  }

  return false;
}

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
