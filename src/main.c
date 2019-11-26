#include <stdio.h>
#include "syntax_analyzer.h"
#include "error.h"

#define DEBUG 1

int main (int argc, char *argv[]) {
    if(DEBUG){
        if (argc == 1) {
            fprintf(stderr, "neni predany zdrojovy parametr\n");
            throw_err(INTERN_ERR);
        }

        FILE *f = fopen(argv[1], "r");

        ASTNode* tree;
        int syntax_result = get_derivation_tree(f, &tree);

        if (syntax_result != 0) {
            // lexikalni, syntakticka nebo interni chyba
            fclose(f);
            throw_err(syntax_result);
        }
        free_tree(tree);
        fclose(f);
    }else{
        ASTNode* tree;
        int syntax_result = get_derivation_tree(stdin, &tree);

        if (syntax_result != 0) {
            // lexikalni, syntakticka nebo interni chyba
            throw_err(syntax_result);
        }

        free_tree(tree);
    }

    return 0;
}
