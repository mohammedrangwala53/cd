#include <stdio.h>
#include <ctype.h>
#include <string.h>

const char *keywords[] = { "int", "return", "if", "else", "while", "for", NULL };

int isKeyword(const char *s) {
    for (int i = 0; keywords[i]; i++)
        if (strcmp(s, keywords[i]) == 0)
            return 1;
    return 0;
}

int main() {
    FILE *f = fopen("input_c.txt", "r");
    if (!f) {
        perror("File open error");
        return 1;
    }

    printf("%-8s %-15s %-15s %-8s\n", "LineNo", "Lexeme", "Token", "TokenNo");
    int line = 1;
    int kw_ct = 0, id_ct = 0, num_ct = 0, op_ct = 0, sep_ct = 0, err_ct = 0;
    int c;

    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') { line++; continue; }
        if (isspace(c)) continue;

        char buf[64];
        int n = 0;

        // Identifier or keyword
        if (isalpha(c)) {
            buf[n++] = c;
            while (isalnum(c = fgetc(f))) buf[n++] = c;
            buf[n] = '\0';
            if (c != EOF) ungetc(c, f);

            if (isKeyword(buf)) {
                kw_ct++;
                printf("%-8d %-15s %-15s %-8d\n", line, buf, "KEYWORD", kw_ct);
            } else {
                id_ct++;
                printf("%-8d %-15s %-15s %-8d\n", line, buf, "IDENTIFIER", id_ct);
            }
        }
        // Number or invalid identifier
        else if (isdigit(c)) {
            buf[n++] = c;
            int hasAlpha = 0;

            while ((c = fgetc(f)), isalnum(c)) {
                if (isalpha(c)) hasAlpha = 1;
                buf[n++] = c;
            }
            buf[n] = '\0';
            if (c != EOF) ungetc(c, f);

            if (hasAlpha) {
                err_ct++;
                printf("%-8d %-15s %-15s %-8d\n", line, buf, "INVALID_ID", err_ct);
            } else {
                num_ct++;
                printf("%-8d %-15s %-15s %-8d\n", line, buf, "NUMBER", num_ct);
            }
        }
        // Operator
        else if (strchr("+-*/=<>!", c)) {
            buf[0] = c; buf[1] = '\0';
            op_ct++;
            printf("%-8d %-15s %-15s %-8d\n", line, buf, "OPERATOR", op_ct);
        }
        // Separator
        else if (strchr(";,(){}", c)) {
            buf[0] = c; buf[1] = '\0';
            sep_ct++;
            printf("%-8d %-15s %-15s %-8d\n", line, buf, "SEPARATOR", sep_ct);
        }
    }

    fclose(f);
    return 0;
}
