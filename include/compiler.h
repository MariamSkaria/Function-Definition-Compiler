
#ifndef COMPILER_H
#define COMPILER_H

typedef enum {
    TOK_INT,
    TOK_FLOAT,
    TOK_VOID,
    TOK_RETURN,
    TOK_IDENTIFIER,
    TOK_NUMBER,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_COMMA,
    TOK_SEMICOLON,
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_EOF,
    TOK_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[128];
    int line;
} Token;

#endif
