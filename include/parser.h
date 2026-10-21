#ifndef PARSER_H
#define PARSER_H
#include "compiler.h"
#include "ast.h"
int parse_tokens(const TokenList *tokens, Program *program, Diagnostic *errors, size_t *error_count);
#endif
