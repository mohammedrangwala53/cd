%{
#include <stdio.h>
int line=1, kw=0, id=0, num=0, str=0, op=0, sep=0;
void P(const char*t,const char*n,int*c){ printf("%-4d %-15s %-12s %-3d\n",line,t,n,++*c); }
%}
%option noyywrap


KEYWORD     SELECT|FROM|WHERE|INSERT|INTO|VALUES|UPDATE|SET|DELETE
ID          [a-zA-Z_][a-zA-Z0-9_]*
NUM         [0-9]+
OP          [+\-*/=<>!]
SEP         [;,()]
STR         \'([^\'\n]*)\'
UNCLOSED    \'[^\']*\n

%%

\n                  { line++; }
{KEYWORD}           { P(yytext,"KEYWORD",&kw); }
{ID}                { P(yytext,"IDENTIFIER",&id); }
{NUM}               { P(yytext,"NUMBER",&num); }
{OP}                { P(yytext,"OPERATOR",&op); }
{SEP}               { P(yytext,"SEPARATOR",&sep); }
{STR}               { P(yytext,"STRING",&str); }
{UNCLOSED}          { printf("%-4d %-15s %-12s %-3s\n", line, yytext, "UNCLOSED_STRING", "ERR"); }
[ \t\r]+            ;
.                   ;

%%

int main() {
    yylex();
    return 0;
}
