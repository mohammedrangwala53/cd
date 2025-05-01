#include <stdio.h>
#include <ctype.h>
#include <string.h>

const char *keywords[] = {
    "SELECT", "FROM", "WHERE", "INSERT", "INTO", "VALUES", "UPDATE", "SET",
    "DELETE", "CREATE", "TABLE", "DROP", "ALTER", "ADD", "JOIN", "ON", "AND",
    "OR", "NOT", "NULL", "AS", "IN", "IS", "LIKE", "DISTINCT", NULL
};

int isKeyword(const char *s) {
    for (int i = 0; keywords[i]; i++)
        if (strcasecmp(s, keywords[i]) == 0)
            return 1;
    return 0;
}

int main() {
    FILE *f = fopen("input_sql.txt", "r");
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

        // Identifiers or Keywords
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
        // Numeric literals
        else if (isdigit(c)) {
            buf[n++] = c;
            while (isdigit(c = fgetc(f))) buf[n++] = c;
            buf[n] = '\0';
            if (c != EOF) ungetc(c, f);

            num_ct++;
            printf("%-8d %-15s %-15s %-8d\n", line, buf, "NUMBER", num_ct);
        }
        // String literals (detect unclosed single quotes)
        else if (c == '\'') {
            buf[n++] = c;
            int closed = 0;
            while ((c = fgetc(f)) != EOF) {
                buf[n++] = c;
                if (c == '\'') {
                    closed = 1;
                    break;
                }
                if (c == '\n') break;
            }
            buf[n] = '\0';
            if (closed) {
                printf("%-8d %-15s %-15s %-8s\n", line, buf, "STRING", "-");
            } else {
                err_ct++;
                printf("%-8d %-15s %-15s %-8d\n", line, buf, "UNCLOSED_STRING", err_ct);
            }
        }
        // Operators
        else if (strchr("=<>!*/+-", c)) {
            buf[0] = c; buf[1] = '\0';
            op_ct++;
            printf("%-8d %-15s %-15s %-8d\n", line, buf, "OPERATOR", op_ct);
        }
        // Separators
        else if (strchr("(),;*", c)) {
            buf[0] = c; buf[1] = '\0';
            sep_ct++;
            printf("%-8d %-15s %-15s %-8d\n", line, buf, "SEPARATOR", sep_ct);
        }
    }

    fclose(f);
    return 0;
}
