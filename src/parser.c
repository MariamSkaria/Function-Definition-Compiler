#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

typedef struct { const TokenList *tokens; size_t pos; Diagnostic *errors; size_t *error_count; } Parser;
static Token *cur(Parser *p) { return (Token *)&p->tokens->items[p->pos < p->tokens->count ? p->pos : p->tokens->count-1]; }
static Token *advance(Parser *p) { Token *t=cur(p); if (t->type != TOK_EOF) p->pos++; return t; }
static int accept(Parser *p, TokenType t) { if (cur(p)->type==t) { advance(p); return 1; } return 0; }
static void error_at(Parser *p, const char *msg) {
    if (*p->error_count >= MAX_DIAGNOSTICS) return;
    Token *t=cur(p); Diagnostic *d=&p->errors[(*p->error_count)++];
    snprintf(d->message,sizeof d->message,"%s (found '%s')",msg,t->lexeme);
    d->line=t->line; d->column=t->column;
}
static int expect(Parser *p, TokenType t, const char *msg) { if (accept(p,t)) return 1; error_at(p,msg); return 0; }
static DataType parse_type(Parser *p) {
    if (accept(p,TOK_INT)) return TYPE_INT;
    if (accept(p,TOK_FLOAT)) return TYPE_FLOAT;
    if (accept(p,TOK_VOID)) return TYPE_VOID;
    error_at(p,"Expected return/parameter type"); if (cur(p)->type!=TOK_EOF) advance(p); return TYPE_ERROR;
}
static Expr *new_expr(ExprKind k, const char *text, TokenType op, Expr *l, Expr *r) {
    Expr *e=(Expr *)calloc(1,sizeof(Expr)); if (!e) return NULL;
    e->kind=k; if (text) snprintf(e->text,sizeof e->text,"%s",text); e->op=op; e->left=l; e->right=r; return e;
}
static Expr *primary(Parser *p) {
    if (cur(p)->type==TOK_NUMBER) { Token *t=advance(p); return new_expr(EXPR_NUMBER,t->lexeme,TOK_INVALID,NULL,NULL); }
    if (cur(p)->type==TOK_IDENTIFIER) { Token *t=advance(p); return new_expr(EXPR_IDENTIFIER,t->lexeme,TOK_INVALID,NULL,NULL); }
    if (accept(p,TOK_LPAREN)) { Expr *e=NULL;
        /* Parenthesized expressions are intentionally limited to a single primary or simple expression. */
        if (cur(p)->type==TOK_NUMBER || cur(p)->type==TOK_IDENTIFIER) e=primary(p);
        else error_at(p,"Expected expression inside parentheses");
        expect(p,TOK_RPAREN,"Expected ')'"); return e;
    }
    error_at(p,"Expected number or identifier"); if (cur(p)->type!=TOK_EOF) advance(p); return NULL;
}
static Expr *expression(Parser *p) {
    Expr *left=primary(p);
    while (cur(p)->type==TOK_PLUS || cur(p)->type==TOK_MINUS || cur(p)->type==TOK_STAR || cur(p)->type==TOK_SLASH) {
        TokenType op=advance(p)->type; Expr *right=primary(p);
        left=new_expr(EXPR_BINARY,NULL,op,left,right);
    }
    return left;
}
static int parse_function(Parser *p, Program *pr) {
    if (pr->function_count>=MAX_FUNCTIONS) { error_at(p,"Too many functions"); return 0; }
    Function *f=&pr->functions[pr->function_count]; memset(f,0,sizeof *f);
    f->return_type=parse_type(p); f->line=cur(p)->line;
    if (cur(p)->type==TOK_IDENTIFIER) snprintf(f->name,sizeof f->name,"%s",advance(p)->lexeme);
    else { error_at(p,"Expected function name"); return 0; }
    if (!expect(p,TOK_LPAREN,"Expected '(' after function name")) return 0;
    if (cur(p)->type!=TOK_RPAREN) {
        do {
            if (f->param_count>=MAX_PARAMS) { error_at(p,"Too many parameters"); break; }
            Parameter *a=&f->params[f->param_count]; a->type=parse_type(p); a->line=cur(p)->line;
            if (cur(p)->type==TOK_IDENTIFIER) snprintf(a->name,sizeof a->name,"%s",advance(p)->lexeme);
            else { error_at(p,"Expected parameter name"); break; }
            f->param_count++;
        } while (accept(p,TOK_COMMA));
    }
    expect(p,TOK_RPAREN,"Expected ')' after parameters");
    if (!expect(p,TOK_LBRACE,"Expected '{' to begin function body")) return 0;
    while (cur(p)->type!=TOK_RBRACE && cur(p)->type!=TOK_EOF && f->statement_count<MAX_STATEMENTS) {
        Statement *st=&f->statements[f->statement_count]; memset(st,0,sizeof *st); st->line=cur(p)->line;
        if (accept(p,TOK_RETURN)) {
            if (accept(p,TOK_SEMICOLON)) st->kind=STMT_RETURN_EMPTY;
            else { st->kind=STMT_RETURN_EXPR; st->expr=expression(p); expect(p,TOK_SEMICOLON,"Expected ';' after return expression"); }
            f->statement_count++;
        } else if (cur(p)->type==TOK_INT || cur(p)->type==TOK_FLOAT) {
            st->kind=STMT_DECLARATION; st->declared_type=parse_type(p);
            if (cur(p)->type==TOK_IDENTIFIER) snprintf(st->name,sizeof st->name,"%s",advance(p)->lexeme);
            else error_at(p,"Expected variable name");
            expect(p,TOK_SEMICOLON,"Expected ';' after declaration"); f->statement_count++;
        } else {
            error_at(p,"Expected return statement or variable declaration");
            while (cur(p)->type!=TOK_SEMICOLON && cur(p)->type!=TOK_RBRACE && cur(p)->type!=TOK_EOF) advance(p);
            accept(p,TOK_SEMICOLON);
        }
    }
    expect(p,TOK_RBRACE,"Expected '}' at end of function");
    pr->function_count++; return 1;
}
int parse_tokens(const TokenList *tokens, Program *program, Diagnostic *errors, size_t *error_count) {
    Parser p={tokens,0,errors,error_count}; memset(program,0,sizeof *program); *error_count=0;
    while (cur(&p)->type!=TOK_EOF) {
        size_t before=p.pos;
        if (!parse_function(&p,program) && p.pos==before) advance(&p);
    }
    return *error_count==0;
}
