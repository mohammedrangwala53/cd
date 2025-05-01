%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex();
%}

%union {
    int num;   /* For numbers */
    char var;  /* For single-letter variables */
}

%token <num> NUMBER
%token <var> VARIABLE
%left '+' '-'
%left '*' '/'

%%

expr : expr '+' term  { printf("Parsed: expr + term\n"); }
     | expr '-' term  { printf("Parsed: expr - term\n"); }
     | term           { printf("Parsed: term\n"); }
     ;

term : term '*' factor { printf("Parsed: term * factor\n"); }
     | term '/' factor { printf("Parsed: term / factor\n"); }
     | factor          { printf("Parsed: factor\n"); }
     ;

factor : '(' expr ')'  { printf("Parsed: (expr)\n"); }
       | NUMBER        { printf("Parsed: NUMBER: %d\n", $1); }
       | VARIABLE      { printf("Parsed: VARIABLE: %c\n", $1); }
       ;

%%

void yyerror(const char *s) {
    printf("Error: %s\n", s);
}

int main() {
    printf("Enter an expression:\n");
    yyparse();
    return 0;
}
