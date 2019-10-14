#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "keywords.h"
#include "lex_analyzer.h"
#include "syntax_analyzer.h"

#define DEBUG 1

int main (int argc, char *argv[]) {
    if(DEBUG){
        FILE *f = fopen(argv[1], "r");
        get_derivation_tree(f);
        fclose(f);
    }else{
        get_next_token(stdin);
    }

    return 0;
}
