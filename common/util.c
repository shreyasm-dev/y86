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

  push((*map).keys, key, (*map).n);
  (*map).n--;
  push((*map).values, value, (*map).n);

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

bool eq(char* a, char* b) {
  if (a == NULL || b == NULL) {
    return false;
  }

  return !strcmp(a, b);
}

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
  int n = 0;

  while ((c = fgetc(file)) != EOF) {
    push(str, c, n);
  }

  fclose(file);

  return str;
}

byte* read_ascii_hex(char* filename, long* n) {
  char* str = read_string(filename);
  int len = strlen(str);
  char* pure = (char*)malloc(0);

  for (int i = 0; i < len; i++) {
    if (str[i] == ';') {
      while (str[i] != '\n' && str[i] != EOF) {
        i++;
      }
    }

    if ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f')) {
      push(pure, str[i], *n)
    }
  }

  if (*n % 2 != 0) {
    error("expected number of bits to be even, got %ld", *n);
  }

  *n /= 2;

  byte* buf = (byte*)malloc(*n * sizeof(byte));

  for (int i = 0; i < *n; i++) {
    char a = pure[i * 2];
    char b = pure[i * 2 + 1];

    if ('0' <= a && a <= '9')
      a -= '0';
    else if ('a' <= a && a <= 'f')
      a -= 'a' - 10;
    else
      error("invalid hex character %c", a);

    if ('0' <= b && b <= '9')
      b -= '0';
    else if ('a' <= b && b <= 'f')
      b -= 'a' - 10;
    else
      error("invalid hex character %c", b);

    buf[i] = (a << 4) | b;
  }

  return buf;
}

bool neg(word n) { return n >> 31; }

int cmp(word a, word b) {
  if (a == b) {
    return 0;
  } else if (neg(a) && !neg(b)) {
    return -1;
  } else if (!neg(a) && neg(b)) {
    return 1;
  } else if (a < b) {
    return -1;
  } else {
    return 1;
  }
}

/*

input: [
  [a, b, c],
  [d, e, f],
] - m x n table (m rows, n columns)

output:

╔═════╦═════╦═════╗
║  a  ║  b  ║  c  ║
╠═════╬═════╬═════╣
║  d  ║  e  ║  f  ║
╚═════╩═════╩═════╝

╔═╦═╗
║ ║ ║
╚═╩═╝

*/
void table(char*** table, int m, int n, bool vertical) {
  int padding = 1;

  int* widths = (int*)malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) widths[i] = 0;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      widths[i] = fmax(widths[i], strlen(table[j][i]));

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      printf(i == 0 && j == 0 ? "╔" : (i == 0 ? "╦" : (j == 0 ? "╠" : "╬")));
      for (int n = 0; n < (2 * padding) + widths[j]; n++) printf("═");
    }
    printf(i == 0 ? "╗" : "╣");

    printf("\n");

    for (int j = 0; j < n; j++) {
      printf("║");

      int slack = widths[j] - strlen(table[i][j]);

      for (int n = 0; n < padding + floor(slack / 2.0); n++) printf(" ");

      if ((vertical && j == 0) || (!vertical && i == 0)) {
        bold("%s", table[i][j]);
      } else {
        printf("%s", table[i][j]);
      }

      for (int n = 0; n < padding + ceil(slack / 2.0); n++) printf(" ");
    }
    printf("║");

    printf("\n");
  }

  for (int i = 0; i < n; i++) {
    printf(i == 0 ? "╚" : "╩");
    for (int n = 0; n < (2 * padding) + widths[i]; n++) printf("═");

    if (i == n - 1) {
      printf("╝");
    }
  }

  printf("\n");
}
