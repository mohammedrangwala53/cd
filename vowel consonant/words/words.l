/* metrics.l */
%{
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// ─────── Feature toggles ──────────────────────────────────────────
// Uncomment only the metrics you want
#define COUNT_LINES
#define COUNT_WORDS
#define COUNT_CHARS
#define COUNT_VOWELS
#define COUNT_CONSONANTS
#define COUNT_COMMENTS

// Uncomment one conversion mode if you want to see converted text
#define CONVERT_UPPER
// #define CONVERT_LOWER
// ──────────────────────────────────────────────────────────────────

int line_count = 0;
int word_count = 0;
int char_count = 0;
int vowel_count = 0;
int consonant_count = 0;
int comment_count = 0;
%}

%option noyywrap

%%

#ifdef COUNT_COMMENTS
"//".*                                { comment_count++; }
"/*"([^*]|\*+[^*/])*\*+"/"            { comment_count++; }
#endif

#ifdef COUNT_WORDS
[a-zA-Z]+                             {
                                          word_count++;
    #ifdef COUNT_VOWELS
        for (int i = 0; yytext[i]; i++) {
            char c = yytext[i];
            if (strchr("AEIOUaeiou", c)) vowel_count++;
            else if (isalpha(c)) consonant_count++;
        }
    #endif
}
#endif

\n                                     {
    #ifdef COUNT_LINES
        line_count++;
    #endif
    #ifdef COUNT_CHARS
        char_count++;
    #endif
    #ifdef CONVERT_UPPER
        putchar('\n');
    #endif
    #ifdef CONVERT_LOWER
        putchar('\n');
    #endif
}

.                                      {
    #ifdef COUNT_CHARS
        char_count++;
    #endif
    #ifdef CONVERT_UPPER
        putchar(toupper(yytext[0]));
    #endif
    #ifdef CONVERT_LOWER
        putchar(tolower(yytext[0]));
    #endif
}

%%

int main() {
    yylex();
    #ifdef COUNT_LINES
        printf("Lines:       %d\n", line_count);
    #endif
    #ifdef COUNT_WORDS
        printf("Words:       %d\n", word_count);
    #endif
    #ifdef COUNT_CHARS
        printf("Characters:  %d\n", char_count);
    #endif
    #if defined(COUNT_VOWELS) && defined(COUNT_CONSONANTS)
        printf("Vowels:      %d\n", vowel_count);
        printf("Consonants:  %d\n", consonant_count);
    #endif
    #ifdef COUNT_COMMENTS
        printf("Comments:    %d\n", comment_count);
    #endif
    return 0;
}
