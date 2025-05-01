%{
#include "calc.tab.h" /* Include the correct header file */
%}

%%

[0-9]+ { yylval = atoi(yytext); return NUMBER; }
[+\-*/] { return yytext[0]; }
"(" { return '('; }
")" { return ')'; }
"\n" { return '\n'; }
[ \t] { /* Ignore whitespace */ }
. { printf("Unknown character: %s\n", yytext); }

%%

int yywrap() {
return 1;
}
