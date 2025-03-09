#include "includes.h"

typedef uint8_t byte;

void lookup_init();
int lookup_get(char* key);
int lookup_set(char* key, int value);

bool eq(char* a, char* b);
char* read_string(char* filename);
