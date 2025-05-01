%{
#include <stdio.h>
#include <string.h>

int string_open = 0; // Track if a string started but not closed
%}


%%
"the"|"The"|"a"|"an"            { printf("ARTICLE\t\t%s\n", yytext); }
"he"|"she"|"it"|"He"|"She"|"It" { printf("PRONOUN\t\t%s\n", yytext); }
"runs"|"eats"|"jumps"|"says"    { printf("VERB\t\t%s\n", yytext); }
"dog"|"cat"|"apple"|"tree"|"park" { printf("NOUN\t\t%s\n", yytext); }
"in"|"on"|"under"               { printf("PREPOSITION\t%s\n", yytext); }

\"([^\"\n]*)\"                  { printf("STRING\t\t%s\n", yytext); }
\"([^\"\n]*)                   {
                                  printf("ERROR\t\tUnclosed string: %s\n", yytext);
                                  string_open = 1;
                               }

[.,!?]                         { printf("PUNCTUATION\t%s\n", yytext); }
[ \t\r\n]+                     { /* skip whitespace */ }

.                              { printf("UNKNOWN\t\t%s\n", yytext); }
%%

int main() {
    yylex();
    if (string_open) {
        fprintf(stderr, "Error: Unclosed string detected in input.\n");
    }
    return 0;
}

int yywrap() {
    return 1;
}