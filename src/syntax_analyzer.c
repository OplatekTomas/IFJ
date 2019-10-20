#include "syntax_analyzer.h"

char token_type[][100] = {
        "ID",
        "ASSIGN",
        "GREATER",
        "LESSER",
        "GREATER_OR_EQ",
        "LESSER_OR_EQ",
        "EQ",
        "NON_EQ",
        "OPEN_PARENTHES",
        "CLOSE_PARENTHES",
        "INT",
        "BOOL",
        "FLOAT",
        "STRING",
        "KEYWORD",
        "INDENT",
        "DEDENT",
        "END_OF_FILE",
        "NIL",
        "ERROR",
        "COLON",
        "COMMA",
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "DOUBLE_DIV"
};

void get_derivation_tree(FILE *source){
    Token t;
    IndentStack is;
    stack_init(&is);
    while((t = get_next_token(source, &is)).type != END_OF_FILE){
        if (t.type == INT) {
            printf("INT %i\n", t.numberVal.i);
        } else if (t.type == FLOAT) {
            printf("FLOAT %f\n", t.numberVal.d);
        } else if (t.type == STRING) {
            printf("STRING '%s'\n", t.stringValue);
        } else if (t.type == KEYWORD) {
            printf("%s\n", KEYWORDS[t.keywordValue]);
        } else if (t.type!= ERROR) {
            printf("%s\n", token_type[t.type]);
        }

    }
}
