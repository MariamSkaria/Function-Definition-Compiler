#ifndef COMPILER_H
#define COMPILER_H

#include <stddef.h>

#define MAX_LEXEME 128
#define MAX_TOKENS 2048
#define MAX_FUNCTIONS 128
#define MAX_PARAMS 32
#define MAX_STATEMENTS 128
#define MAX_DIAGNOSTICS 128

typedef enum {
    TOK_INT, TOK_FLOAT, TOK_VOID, TOK_RETURN,
    TOK_IDENTIFIER, TOK_NUMBER,
    TOK_LPAREN, TOK_RPAREN, TOK_LBRACE, TOK_RBRACE,
    TOK_COMMA, TOK_SEMICOLON, TOK_PLUS, TOK_MINUS, TOK_STAR, TOK_SLASH,
    TOK_ASSIGN, TOK_EOF, TOK_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_LEXEME];
    int line;
    int column;
} Token;

typedef struct {
    char message[256];
    int line;
    int column;
} Diagnostic;

typedef struct {
    Token items[MAX_TOKENS];
    size_t count;
} TokenList;

const char *token_type_name(TokenType type);

#endif
