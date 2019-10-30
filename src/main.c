#include <stdio.h>
#include <stdlib.h>
#include "syntax_analyzer.h"

#define DEBUG 1

int main (int argc, char *argv[]) {
    if(DEBUG){
        if (argc == 1) {
            fprintf(stderr, "neni predany zdrojovy parametr\n");
            exit(99);
        }
        FILE *f = fopen(argv[1], "r");
        get_derivation_tree(f);
        fclose(f);
    }else{
        get_derivation_tree(stdin);
    }

    return 0;
}
