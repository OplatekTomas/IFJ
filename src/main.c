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
        ASTNode* tree = get_derivation_tree(f);
        if (tree == NULL) {
            fprintf(stderr, "chyba pri syntakticke analyze\n");
        }
        fclose(f);
    }else{
        ASTNode* tree = get_derivation_tree(stdin);
        if (tree == NULL) {
            fprintf(stderr, "chyba pri syntakticke analyze\n");
        }
    }

    return 0;
}
