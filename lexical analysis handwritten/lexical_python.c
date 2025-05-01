#include <stdio.h>
#include <ctype.h>
#include <string.h>

const char *keywords[] = {
    "def", "return", "if", "else", "elif", "while", "for", "class", "try", 
    "except", "finally", "import", "from", "as", "global", "nonlocal", "and", 
    "or", "not", "in", "is", "True", "False", "None", "lambda", "yield", NULL
};

int isKeyword(const char *s) {
    for (int i = 0; keywords[i]; i++)
        if (strcmp(s, keywords[i]) == 0)
            return 1;
    return 0;
}

int isOperatorChar(char c) {
    return strchr("+-*/%=!<>&|^", c) != NULL;
}

int main() {
    FILE *f = fopen("input_python.txt", "r");
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

        char buf[128];
        int n = 0;

        // Identifier or keyword
        if (isalpha(c) || c == '_') {
            buf[n++] = c;
            while ((c = fgetc(f)), isalnum(c) || c == '_') {
                buf[n++] = c;
            }
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
        // Invalid identifier: starts with digit followed by alphanumeric (e.g., 3value)
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
        else if (isOperatorChar(c)) {
            buf[n++] = c;
            int next = fgetc(f);
            if (isOperatorChar(next)) {
                buf[n++] = next;
                int third = fgetc(f);
                if (isOperatorChar(third)) {
                    buf[n++] = third;
                } else if (third != EOF) {
                    ungetc(third, f);
                }
            } else if (next != EOF) {
                ungetc(next, f);
            }
            buf[n] = '\0';

            op_ct++;
            printf("%-8d %-15s %-15s %-8d\n", line, buf, "OPERATOR", op_ct);
        }
        // Separator (e.g., colon, comma, parentheses)
        else if (strchr("():,[]{}.", c)) {
            buf[0] = c; buf[1] = '\0';
            sep_ct++;
            printf("%-8d %-15s %-15s %-8d\n", line, buf, "SEPARATOR", sep_ct);
        }
    }

    fclose(f);
    return 0;
}
