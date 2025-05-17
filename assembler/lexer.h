#include "../common/includes.h"

#ifndef LEXER_H
#define LEXER_H

void tokenise(char*** tokens, int* n, char* source, bool include_punctuation);

#endif
