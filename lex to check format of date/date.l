%{
#include <stdio.h>
int valid_dates = 0, invalid_dates = 0;
FILE *yyin;
%}

%option noyywrap

%%

([0][1-9]|[12][0-9]|3[01])[-/]([0][1-9]|1[0-2])[-/](19|20)[0-9]{2}   {
    printf("Valid date: %s\n", yytext);
    valid_dates++;
}

[0-9]{2}[-/][0-9]{2}[-/][0-9]{2,3}   {
    printf("Invalid date: %s\n", yytext);
    invalid_dates++;
}

[ \t\r\n]+    { /* skip whitespace */ }
.             { /* skip any other character */ }

%%

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("File open error");
        return 1;
    }

    yyin = fp;
    yylex();
    fclose(fp);

    printf("\nSummary:\n");
    printf("Valid dates   : %d\n", valid_dates);
    printf("Invalid dates : %d\n", invalid_dates);
    return 0;
}
