#include "syntax_analyzer.h"

char token_type[][100] = {
        "ID",
        "COMP",
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
        "ERROR"
};

void get_derivation_tree(FILE *source){
    Token t;
    IndentStack is;
    stack_init(&is);
    while((t = get_next_token(source, &is)).type != END_OF_FILE){
        if (t.type!= ERROR) {
            printf("%s\n", token_type[t.type]);
        }
    }
}
