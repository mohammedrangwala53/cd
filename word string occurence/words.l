%{
#include <stdio.h>
#include <string.h>

int word_count = 0;   // Variable to count the occurrences of the word
char word_to_find[] = "example";  // Word to search for, change as needed
%}

%option noyywrap

%%

[a-zA-Z]+ {
    if (strcmp(yytext, word_to_find) == 0) {
        word_count++;  // Increment count if the word matches
    }
}

.|\n  /* Ignore other characters */

%%

int main() {
    yylex();
    printf("The word '%s' appears %d times.\n", word_to_find, word_count);
    return 0;
}
