%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
void yyerror(const char *s);
%}

/* Define operator precedence */
%left '+' '-'
%left '*' '/'
%right UMINUS /* Unary minus has the highest precedence */

%token NUMBER

%%

input:
input expression '\n' { printf("Result: %d\n", $2); }
| expression '\n' { printf("Result: %d\n", $1); }
;

expression:
expression '+' expression { $$ = $1 + $3; }
| expression '-' expression { $$ = $1 - $3; }
| expression '*' expression { $$ = $1 * $3; }
| expression '/' expression { 
if ($3 == 0) {
yyerror("Error: Division by zero");
$$ = 0;
} else {
$$ = $1 / $3;
}
}
| '-' expression %prec UMINUS { $$ = -$2; } /* Unary minus */
| '(' expression ')' { $$ = $2; }
| NUMBER { $$ = $1; }
;

%%

void yyerror(const char *s) {
fprintf(stderr, "Error: %s\n", s);
}

int main() {
printf("Enter an expression:\n");
yyparse();
return 0;
}
