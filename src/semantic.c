#include <stdio.h>
#include <string.h>
#include "semantic.h"
static void add_error(Diagnostic *e,size_t *n,int line,const char *msg) {
    if (*n>=MAX_DIAGNOSTICS) return;
    snprintf(e[*n].message,sizeof e[*n].message,"%s",msg); e[*n].line=line; e[*n].column=1; (*n)++;
}
static int expr_has_identifier(const Expr *e,const char *name) {
    if (!e) return 0;
    if (e->kind==EXPR_IDENTIFIER && strcmp(e->text,name)==0) return 1;
    return expr_has_identifier(e->left,name)||expr_has_identifier(e->right,name);
}
static void check_expr(const Expr *e,const Function *f,Diagnostic *errors,size_t *n,int line) {
    if (!e) return;
    if (e->kind==EXPR_IDENTIFIER) {
        int found=0;
        for(size_t i=0;i<f->param_count;i++) if(strcmp(f->params[i].name,e->text)==0) found=1;
        for(size_t i=0;i<f->statement_count;i++) if(f->statements[i].kind==STMT_DECLARATION && strcmp(f->statements[i].name,e->text)==0) found=1;
        if(!found) { char msg[256]; snprintf(msg,sizeof msg,"Use of undeclared identifier '%s'",e->text); add_error(errors,n,line,msg); }
    }
    check_expr(e->left,f,errors,n,line); check_expr(e->right,f,errors,n,line);
}
int analyze_semantics(const Program *p,Diagnostic *errors,size_t *n) {
    *n=0;
    for(size_t i=0;i<p->function_count;i++) {
        const Function *f=&p->functions[i];
        if(f->return_type==TYPE_VOID) {
            for(size_t j=0;j<f->statement_count;j++) if(f->statements[j].kind==STMT_RETURN_EXPR)
                add_error(errors,n,f->statements[j].line,"Void function should use 'return;' without a value");
        }
        for(size_t a=0;a<f->param_count;a++) for(size_t b=a+1;b<f->param_count;b++)
            if(strcmp(f->params[a].name,f->params[b].name)==0) {
                char msg[256]; snprintf(msg,sizeof msg,"Duplicate parameter '%s' in function '%s'",f->params[a].name,f->name);
                add_error(errors,n,f->params[b].line,msg);
            }
        for(size_t j=0;j<f->statement_count;j++) {
            const Statement *s=&f->statements[j];
            if(s->kind==STMT_RETURN_EXPR) check_expr(s->expr,f,errors,n,s->line);
        }
        for(size_t j=i+1;j<p->function_count;j++) if(strcmp(f->name,p->functions[j].name)==0) {
            char msg[256]; snprintf(msg,sizeof msg,"Duplicate function name '%s'",f->name); add_error(errors,n,p->functions[j].line,msg);
        }
        (void)expr_has_identifier;
    }
    return *n==0;
}
