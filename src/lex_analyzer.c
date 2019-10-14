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

// Vraci:
// 0 - kdyz jen zkonzumuje mezery a nechce vratit token
// 1 - kdyz chce vratit token
// 2 - kdyz najde chybu
int handle_indent(FILE* source, indentStack *is, token* t){
    int indent = count_spaces(source);

    // je stejny indent
    if (indent == stack_top(is)) {
        return false;
    } else if (indent > stack_top(is)) {
        // je vetsi indent nez predtim
        stack_push(is, indent);
        t->type = INDENT;
        return true;
    } else if (indent < stack_top(is)) {
        stack_pop(is);
        if (stack_top(is) < indent) {
            return 2;
        } else if (stack_top(is) > indent) {
            for (int i = 0; i < indent; i++) {
                ungetc(' ', source);
            }
        }
        t->type = DEDENT;
        return true;
    }
    // je jiny indent nez na topu
    /*if (indent != stack_top(is)) {
        // pridani indentu
        if (indent > stack_top(is)) {
            stack_push(is, indent);
            t->type = INDENT;
            return true;
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
    }*/
}

void handle_singleline_comments(FILE * source) {
    do {
        char c = (char)getc(source);
        if (c == EOF) {
            ungetc(EOF, source);
        } else if (c == '\n') {
            break;
        }
    } while (true);
}

void handle_eof(FILE* source, indentStack* is, token* t) {
    if (stack_top(is) == 0) {
        t->type = END_OF_FILE;
    } else {
        stack_pop(is);
        t->type = DEDENT;
        ungetc(EOF, source);
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
            case '\n':;
                int value = handle_indent(source, &is, &t);
                if (value == 0) {
                    continue;
                } else if (value == 1) {
                    return t;
                } else {
                    // chyba
                }
            case '"':
                break;
            case '#':
                handle_singleline_comments(source);
                break;
            case EOF:
                handle_eof(source, &is, &t);
                return t;
            default:
                //if((c >= 'a' && c <= 'z') || )
                break;
        }
    } while (t.type == END_OF_FILE);

    return t;
    // pridani dedentu na konci pro vyrovani s indentama
    /*while (stack_top(&is) != 0) {
        stack_pop(&is);
        printf("DEDENT\n");
    }*/
}
