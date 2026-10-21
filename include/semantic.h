#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "ast.h"
#include "compiler.h"
int analyze_semantics(const Program *program, Diagnostic *errors, size_t *error_count);
#endif
