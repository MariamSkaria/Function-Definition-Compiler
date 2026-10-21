#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#include "lexer.h"
#include "parser.h"
#include "semantic.h"

static char *read_all(FILE *f) {
    size_t cap=4096,n=0; char *s=(char *)malloc(cap); if(!s)return NULL; int c;
    while((c=fgetc(f))!=EOF) { if(n+2>cap){cap*=2; char *tmp=(char *)realloc(s,cap);if(!tmp){free(s);return NULL;}s=tmp;}s[n++]=(char)c; }
    s[n]='\0'; return s;
}
static void print_diagnostics(const char *stage,const Diagnostic *d,size_t n) {
    for(size_t i=0;i<n;i++) printf("%s:%d:%d: %s\n",stage,d[i].line,d[i].column,d[i].message);
}
int main(int argc,char **argv) {
    FILE *f=stdin;
    if(argc>2){fprintf(stderr,"Usage: %s [source-file]\n",argv[0]);return 2;}
    if(argc==2){f=fopen(argv[1],"rb");if(!f){perror(argv[1]);return 2;}}
    char *source=read_all(f); if(argc==2)fclose(f); if(!source){fprintf(stderr,"Unable to read source\n");return 2;}
    TokenList tokens; Diagnostic errors[MAX_DIAGNOSTICS]; size_t n=0;
    lex_source(source,&tokens,errors,&n);
    printf("== TOKENS ==\n");
    for(size_t i=0;i<tokens.count;i++) printf("%-12s %-16s line %d\n",token_type_name(tokens.items[i].type),tokens.items[i].lexeme,tokens.items[i].line);
    if(n){print_diagnostics("LEXER",errors,n);free(source);return 1;}
    Program program;
    if(!parse_tokens(&tokens,&program,errors,&n)){print_diagnostics("PARSER",errors,n);free(source);return 1;}
    printf("\n== PARSE SUMMARY ==\nParsed %zu function definition(s).\n",program.function_count);
    for(size_t i=0;i<program.function_count;i++) {
        Function *fn=&program.functions[i];
        printf("Function %s: returns %s, %zu parameter(s), %zu statement(s)\n",fn->name,data_type_name(fn->return_type),fn->param_count,fn->statement_count);
    }
    if(!analyze_semantics(&program,errors,&n)){print_diagnostics("SEMANTIC",errors,n);free(source);return 1;}
    printf("\nSUCCESS: lexical, syntax, and semantic checks passed.\n");
    free(source); return 0;
}
