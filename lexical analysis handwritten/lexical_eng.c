#include <stdio.h>
#include <ctype.h>
#include <string.h>

const char *keywords[] = {
    "is", "are", "can", "must", "should", "was", "were", "be", NULL
};

int isKeyword(const char *s) {
    for (int i = 0; keywords[i]; i++) {
        if (strcmp(s, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isNoun(const char *s) {
    // A noun starts with an uppercase letter.
    return isupper(s[0]);
}

int isVerb(const char *s) {
    // A simple verb check for a limited set of verbs.
    const char *verbs[] = {"run", "jump", "walk", "eat", "sleep", NULL};
    for (int i = 0; verbs[i]; i++) {
        if (strcmp(s, verbs[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isAdjective(const char *s) {
    // A simple check for adjectives
    const char *adjectives[] = {"big", "small", "good", "bad", NULL};
    for (int i = 0; adjectives[i]; i++) {
        if (strcmp(s, adjectives[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isPunctuation(char c) {
    return c == '.' || c == ',' || c == '?';
}

int main() {
    FILE *f = fopen("input_eng.txt", "r");
    if (!f) {
        perror("File open error");
        return 1;
    }

    printf("%-8s %-15s %-15s %-8s\n", "LineNo", "Lexeme", "Token", "TokenNo");
    int line = 1;
    int kw_ct = 0, noun_ct = 0, verb_ct = 0, adj_ct = 0, id_ct = 0, punc_ct = 0, err_ct = 0;
    int c;

    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') { line++; continue; }
        if (isspace(c)) continue;

        char buf[128];
        int n = 0;

        // Word (identifier, keyword, noun, verb, or adjective)
        if (isalpha(c)) {
            buf[n++] = c;
            while ((c = fgetc(f)), isalnum(c)) {
                buf[n++] = c;
            }
            buf[n] = '\0';
            if (c != EOF) ungetc(c, f);

            if (isKeyword(buf)) {
                kw_ct++;
                printf("%-8d %-15s %-15s %-8d\n", line, buf, "KEYWORD", kw_ct);
            } else if (isNoun(buf)) {
                noun_ct++;
                printf("%-8d %-15s %-15s %-8d\n", line, buf, "NOUN", noun_ct);
            } else if (isVerb(buf)) {
                verb_ct++;
                printf("%-8d %-15s %-15s %-8d\n", line, buf, "VERB", verb_ct);
            } else if (isAdjective(buf)) {
                adj_ct++;
                printf("%-8d %-15s %-15s %-8d\n", line, buf, "ADJECTIVE", adj_ct);
            } else {
                id_ct++;
                printf("%-8d %-15s %-15s %-8d\n", line, buf, "IDENTIFIER", id_ct);
            }
        }
        // Punctuation mark (., ?, etc.)
        else if (isPunctuation(c)) {
            buf[0] = c; buf[1] = '\0';
            punc_ct++;
            printf("%-8d %-15s %-15s %-8d\n", line, buf, "PUNCTUATION", punc_ct);
        } else {
            // Error handling for unknown lexemes
            err_ct++;
            buf[0] = c;
            buf[1] = '\0';
            printf("%-8d %-15s %-15s %-8d\n", line, buf, "ERROR", err_ct);
        }
    }

    fclose(f);
    return 0;
}
