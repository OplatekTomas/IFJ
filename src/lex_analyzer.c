//
// Created by tomas on 14.10.19.
//

#include "lex_analyzer.h"


int count_spaces(FILE* source) {
    int spaces = 0;
    char c;
    do {
        c = (char)getc(source);
        if (c == ' ') {
            spaces++;
        } else {
            ungetc(c, source);
            break;
        }
    } while (true);
    return spaces;
}

void handle_indent(FILE* source, indentStack *is){
    int indent = count_spaces(source);
    // je jiny indent nez na topu
    if (indent != stack_top(is)) {
        // pridani indentu
        if (indent > stack_top(is)) {
            stack_push(is, indent);
            printf("INDENT\n");
        } else if (indent < stack_top(is)) {
            // ubrani indentu
            do {
                stack_pop(is);
                printf("DEDENT\n");
                // jsme na spravne urovni dedentu?
                if (stack_top(is) == indent) {
                    break;
                } else if (stack_top(is) < indent) {
                    // indent muze byt mensi nez top jedine v pripade ze se popovalo a nenasla se hodnota topu, takze nastala indent chyba
                    exit(1);
                }
            } while (stack_top(is) != 0);
        }
    }
}

token get_next_token(FILE* source){
    token t;
    char c;
    indentStack is;
    stack_init(&is);
    // cteni souboru znak po znaku
    do {
        c = (char)getc(source);
        switch (c) {
            case '\n':
                handle_indent(source, &is);
                break;
            case '"':
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

}