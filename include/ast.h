#ifndef AST_H
#define AST_H
#include "compiler.h"

typedef enum { TYPE_INT, TYPE_FLOAT, TYPE_VOID, TYPE_ERROR } DataType;
typedef enum { EXPR_NUMBER, EXPR_IDENTIFIER, EXPR_BINARY } ExprKind;
typedef struct Expr {
    ExprKind kind;
    char text[MAX_LEXEME];
    TokenType op;
    struct Expr *left;
    struct Expr *right;
} Expr;
typedef struct { DataType type; char name[MAX_LEXEME]; int line; } Parameter;
typedef enum { STMT_RETURN_EXPR, STMT_RETURN_EMPTY, STMT_DECLARATION } StatementKind;
typedef struct {
    StatementKind kind;
    DataType declared_type;
    char name[MAX_LEXEME];
    Expr *expr;
    int line;
} Statement;
typedef struct {
    DataType return_type;
    char name[MAX_LEXEME];
    Parameter params[MAX_PARAMS];
    size_t param_count;
    Statement statements[MAX_STATEMENTS];
    size_t statement_count;
    int line;
} Function;
typedef struct {
    Function functions[MAX_FUNCTIONS];
    size_t function_count;
} Program;
void free_expr(Expr *expr);
const char *data_type_name(DataType type);
#endif
