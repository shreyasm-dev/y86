#include "includes.h"
#include "util.h"

// simple lexer: just split it by whitespace, parentheses, and commas
void tokenise(char*** tokens, int* n, char* source) {
  *tokens = (char**)malloc(0);
  *n = 0;

  char* current = (char*)malloc(0);
  int m = 0;

  for (int i = 0; i < strlen(source); i++) {
    if (ceq_any(_, " \n\t\r(),")) {
      if (eq(current, "")) {
        continue;
      }

      // add current to tokens
      push(*tokens, current, *n);

      // reset current
      current = (char*)malloc(0);
      m = 0;
    } else {
      // append to current
      push(current, _, m);
    }
  }

  if (!eq(current, "")) {
    push(*tokens, current, *n);
  }
}
