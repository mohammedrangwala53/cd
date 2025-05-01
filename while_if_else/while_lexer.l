%{
#include <stdio.h>
#include <string.h>
#include "while_parser.tab.h"


int lineno = 1;
%}

%%
[ \t]           { /* Ignore whitespace */ }
[\n]            { lineno++; }
"while"         { return WHILE; }
"if"            { return IF; }
"else"          { return ELSE; }
"for"           { return FOR; }
"return"        { return RETURN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
"("             { return LPAREN; }
")"             { return RPAREN; }
";"             { return SEMICOLON; }
","             { return COMMA; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULT; }
"/"             { return DIV; }
"%"             { return MOD; }
"="             { return ASSIGN; }
"=="            { return EQ; }
"!="            { return NE; }
"<"             { return LT; }
">"             { return GT; }
"<="            { return LE; }
">="            { return GE; }
"&&"            { return AND; }
"||"            { return OR; }
"!"             { return NOT; }
[0-9]+          { return NUM; }
[a-zA-Z][a-zA-Z0-9_]* { return ID; }
\/\/.*          { /* Ignore comments */ }
.               { printf("Unrecognized character: %s\n", yytext); }
%%

int yywrap() {
    return 1;
}
