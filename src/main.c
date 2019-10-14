#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "keywords.h"

int count_spaces(FILE* in) {
    int spaces = 0;
    char c;
    do {
        c = (char)getc(in);
        if (c == ' ') {
            spaces++;
        } else {
            ungetc(c, in);
            break;
        }
    } while (true);
    return spaces;
}

int main () {
    char c;
    IndentStack is;
    stack_init(&is);
    // cteni souboru znak po znaku
    do {
        c = (char)getc(stdin);
        switch (c) {
            case '\n':;
                // je novy radek
                // multiline stringy rozbijou rozpoznavani indentu
                int indent = count_spaces(stdin);
                // je jiny indent nez na topu
                if (indent != stack_top(&is)) {
                    // pridani indentu
                    if (indent > stack_top(&is)) {
                        stack_push(&is, indent);
                        printf("INDENT\n");
                    } else if (indent < stack_top(&is)) {
                        // ubrani indentu
                        do {
                            stack_pop(&is);
                            printf("DEDENT\n");
                            // jsme na spravne urovni dedentu?
                            if (stack_top(&is) == indent) {
                                break;
                            } else if (stack_top(&is) < indent) {
                                // indent muze byt mensi nez top jedine v pripade ze se popovalo a nenasla se hodnota topu, takze nastala indent chyba
                                exit(1);
                            }
                        } while (stack_top(&is) != 0);
                    }
                }
                break;
            case '"':;
                break;
            default:
                break;
        }
    } while (c != EOF);

    // pridani dedentu na konci pro vyrovani s indentama
    while (stack_top(&is) != 0) {
        stack_pop(&is);
        printf("DEDENT\n");
    }
    return 0;
}
