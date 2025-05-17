#include "../common/includes.h"

// simple lexer: just splits it by whitespace, parentheses, and commas
void tokenise(char*** tokens, int* n, char* source, bool include_punctuation) {
  *tokens = (char**)malloc(0);
  *n = 0;

  char* current = (char*)malloc(0);
  int m = 0;

  for (int i = 0; i < strlen(source); i++) {
    if (ceq_any(_, " \n\t\r(),")) {
      if (!eq(current, "")) {
        // add current to tokens
        push(*tokens, current, *n);

        // reset current
        current = (char*)malloc(0);
        m = 0;
      }

      if (include_punctuation) {
        // add _ to tokens
        char* _str = (char*)malloc(2);
        snprintf(_str, sizeof(_str), "%c", _);
        push(*tokens, _str, *n);
      }
    } else {
      // append to current
      push(current, _, m);
    }
  }

  if (!eq(current, "")) {
    push(*tokens, current, *n);
  }
}
