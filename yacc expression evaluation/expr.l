%{
#include "expr.tab.h"  // Include the generated header file
%}

%%

[0-9]+       { yylval.num = atoi(yytext); return NUMBER; }
[a-zA-Z]     { yylval.var = yytext[0]; return VARIABLE; }
[+\-*/()]    { return yytext[0]; }
[ \t\n]      { /* Ignore whitespace */ }
.            { printf("Invalid character: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}
