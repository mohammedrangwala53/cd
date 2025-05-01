%{
#include "sql.tab.h"
#include <string.h>
%}

DIGIT      [0-9]
ID         [a-zA-Z_][a-zA-Z0-9_]*
WS         [ \t\r\n]+

%%

"SELECT"           { return SELECT; }
"FROM"             { return FROM; }
"WHERE"            { return WHERE; }
"AND"              { return AND; }
"OR"               { return OR; }
"*"                { return STAR; }
","                { return COMMA; }
"="                { return EQ; }
"<="               { return LE; }
">="               { return GE; }
"<>"               { return NEQ; }
"<"                { return LT; }
">"                { return GT; }
"("                { return LPAREN; }
")"                { return RPAREN; }
\'([^\\\n]|(\\.))*?\'   { yylval.str = strdup(yytext); return STRING; }
{ID}               { yylval.str = strdup(yytext); return IDENTIFIER; }
{DIGIT}+           { yylval.str = strdup(yytext); return NUMBER; }
{WS}               { /* skip whitespace */ }
.                  { fprintf(stderr, "Invalid character: %s\n", yytext); return INVALID; }

%%

int yywrap(void) { return 1; }
