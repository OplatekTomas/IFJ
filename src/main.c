#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "keywords.h"
#include "lex_analyzer.h"

#define DEBUG 0

int main (int argc, char *argv[]) {
    if(DEBUG){
        FILE *f = fopen(argv[1], "r");
        get_next_token(f);
        fclose(f);
    }else{
        get_next_token(stdin);
    }

    return 0;
}
