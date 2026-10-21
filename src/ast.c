#include <stdlib.h>
#include "ast.h"
void free_expr(Expr *e) { if (!e) return; free_expr(e->left); free_expr(e->right); free(e); }
const char *data_type_name(DataType t) {
    switch (t) { case TYPE_INT: return "int"; case TYPE_FLOAT: return "float"; case TYPE_VOID: return "void"; default: return "error"; }
}
