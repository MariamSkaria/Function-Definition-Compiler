#ifndef LEXER_H
#define LEXER_H
#include "compiler.h"
int lex_source(const char *source, TokenList *out, Diagnostic *errors, size_t *error_count);
#endif
