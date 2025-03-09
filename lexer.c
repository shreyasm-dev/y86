#include "includes.h"
#include "util.h"

// simple lexer: just split it by whitespace and parentheses
void tokenise(char*** tokens, int* n, char* source) {
  *tokens = (char**)malloc(0);
  *n = 0;

  char* current = (char*)malloc(0);

  for (int i = 0; i < strlen(source); i++) {
    if (source[i] == ' ' || source[i] == '\n' || source[i] == '\t' ||
        source[i] == '\r' || source[i] == '(' || source[i] == ')') {
      if (eq(current, "")) {
        continue;
      }

      // add current to tokens
      *tokens = (char**)realloc(*tokens, (*n + 1) * sizeof(char*));
      (*tokens)[(*n)++] = current;

      // reset current
      current = (char*)malloc(0);
    } else {
      // append to current
      current = (char*)realloc(current, (strlen(current) + 1) * sizeof(char));
      current[strlen(current)] = source[i];
    }
  }

  if (!eq(current, "")) {
    *tokens = (char**)realloc(*tokens, (*n + 1) * sizeof(char*));
    (*tokens)[(*n)++] = current;
  }
}
