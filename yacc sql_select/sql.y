%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s);
int yylex(void);
FILE *yyin;
%}

%union {
    char *str;
}

%token <str> IDENTIFIER NUMBER STRING
%token SELECT FROM WHERE AND OR STAR COMMA EQ LT GT LE GE NEQ LPAREN RPAREN INVALID

%nonassoc LOWER_THAN_WHERE
%left OR
%left AND
%nonassoc EQ LT GT LE GE NEQ

%start statement

%%

statement:
    SELECT select_list FROM table_list opt_where_clause
        { printf("Valid SELECT statement.\n"); }
;

select_list:
    STAR
  | column_list
;

column_list:
    column_list COMMA IDENTIFIER
  | IDENTIFIER
;

table_list:
    table_list COMMA IDENTIFIER
  | IDENTIFIER
;

opt_where_clause:
    /* empty */
  | WHERE condition
;

condition:
    IDENTIFIER comparison_op value
  | LPAREN condition RPAREN
  | condition AND condition
  | condition OR condition
;

comparison_op:
    EQ
  | LT
  | GT
  | LE
  | GE
  | NEQ
;

value:
    IDENTIFIER
  | NUMBER
  | STRING
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    yyin = fp;
    int result = yyparse();
    fclose(fp);
    return result;
}
