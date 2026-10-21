#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"

static void add_error(Diagnostic *e, size_t *n, int line, int col, const char *msg) {
    if (*n >= MAX_DIAGNOSTICS) return;
    snprintf(e[*n].message, sizeof e[*n].message, "%s", msg);
    e[*n].line = line; e[*n].column = col; (*n)++;
}
static int push(TokenList *out, TokenType type, const char *start, size_t len, int line, int col) {
    if (out->count >= MAX_TOKENS) return 0;
    Token *t = &out->items[out->count++];
    t->type = type; t->line = line; t->column = col;
    if (len >= MAX_LEXEME) len = MAX_LEXEME - 1;
    memcpy(t->lexeme, start, len); t->lexeme[len] = '\0';
    return 1;
}
const char *token_type_name(TokenType t) {
    switch (t) {
        case TOK_INT: return "INT"; case TOK_FLOAT: return "FLOAT"; case TOK_VOID: return "VOID";
        case TOK_RETURN: return "RETURN"; case TOK_IDENTIFIER: return "IDENTIFIER";
        case TOK_NUMBER: return "NUMBER"; case TOK_LPAREN: return "LPAREN";
        case TOK_RPAREN: return "RPAREN"; case TOK_LBRACE: return "LBRACE";
        case TOK_RBRACE: return "RBRACE"; case TOK_COMMA: return "COMMA";
        case TOK_SEMICOLON: return "SEMICOLON"; case TOK_PLUS: return "PLUS";
        case TOK_MINUS: return "MINUS"; case TOK_STAR: return "STAR"; case TOK_SLASH: return "SLASH";
        case TOK_ASSIGN: return "ASSIGN"; case TOK_EOF: return "EOF"; default: return "INVALID";
    }
}
int lex_source(const char *s, TokenList *out, Diagnostic *errors, size_t *error_count) {
    size_t i = 0; int line = 1, col = 1; out->count = 0; *error_count = 0;
    while (s[i]) {
        if (isspace((unsigned char)s[i])) { if (s[i] == '\n') { line++; col = 1; } else col++; i++; continue; }
        int start_line = line, start_col = col; size_t start = i;
        if (isalpha((unsigned char)s[i]) || s[i] == '_') {
            while (isalnum((unsigned char)s[i]) || s[i] == '_') { i++; col++; }
            size_t n = i - start; TokenType type = TOK_IDENTIFIER;
            if (n == 3 && strncmp(s+start,"int",3)==0) type=TOK_INT;
            else if (n == 5 && strncmp(s+start,"float",5)==0) type=TOK_FLOAT;
            else if (n == 4 && strncmp(s+start,"void",4)==0) type=TOK_VOID;
            else if (n == 6 && strncmp(s+start,"return",6)==0) type=TOK_RETURN;
            if (!push(out,type,s+start,n,start_line,start_col)) return 0;
            continue;
        }
        if (isdigit((unsigned char)s[i])) {
            int seen_dot = 0;
            while (isdigit((unsigned char)s[i]) || (s[i]=='.' && !seen_dot)) {
                if (s[i]=='.') seen_dot=1;
                i++; col++;
            }
            if (!push(out,TOK_NUMBER,s+start,i-start,start_line,start_col)) return 0;
            continue;
        }
        TokenType type;
        switch (s[i]) {
            case '(': type=TOK_LPAREN; break; case ')': type=TOK_RPAREN; break;
            case '{': type=TOK_LBRACE; break; case '}': type=TOK_RBRACE; break;
            case ',': type=TOK_COMMA; break; case ';': type=TOK_SEMICOLON; break;
            case '+': type=TOK_PLUS; break; case '-': type=TOK_MINUS; break;
            case '*': type=TOK_STAR; break; case '/': type=TOK_SLASH; break; case '=': type=TOK_ASSIGN; break;
            default: {
                char msg[100]; snprintf(msg,sizeof msg,"Invalid character '%c'",s[i]);
                add_error(errors,error_count,line,col,msg);
                push(out,TOK_INVALID,s+i,1,line,col); i++; col++; continue;
            }
        }
        push(out,type,s+i,1,line,col); i++; col++;
    }
    push(out,TOK_EOF,"",0,line,col);
    return 1;
}
