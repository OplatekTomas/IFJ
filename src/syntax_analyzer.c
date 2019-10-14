#include "syntax_analyzer.h"

char token_type[10][100] = {"ID", "COMP", "OPEN_PARENTHES", "CLOSE_PARENTHES", "INT", "BOOL", "FLOAT", "STRING", "NIL", "END_OF_FILE"};

void get_derivation_tree(FILE *source){
    token t;
    while((t = get_next_token(source)).type != END_OF_FILE){
        printf("%s\n", token_type[t.type]);
    }
}