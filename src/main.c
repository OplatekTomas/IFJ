#include <stdio.h>
#include "syntax_analyzer.h"
#include "semantic_analyzer.h"
#include "error.h"

#define DEBUG 1

int main (int argc, char *argv[]) {
    if(DEBUG){
        if (argc == 1) {
            fprintf(stderr, "neni predany zdrojovy parametr\n");
            throw_err(INTERN_ERR);
        }

        FILE *f = fopen(argv[1], "r");

        if (f == NULL) {
            fprintf(stderr, "neexistujici soubor\n");
            throw_err(INTERN_ERR);
        }

        ASTNode* tree;
        int syntax_result = get_derivation_tree(f, &tree);

        if (syntax_result != 0) {
            // lexikalni, syntakticka nebo interni chyba
            fclose(f);
            throw_err(syntax_result);
        }
        print_tree(tree);
        int semantics_result = check_semantics(tree);

        if (semantics_result != 0) {
            // semanticka chyba nebo interni chyba
            fclose(f);
            free_tree(tree);
            throw_err(semantics_result);
        }
        free_tree(tree);
        fclose(f);
    }else{
        ASTNode* tree;
        int syntax_result = get_derivation_tree(stdin, &tree);

        if (syntax_result != 0) {
            // lexikalni, syntakticka nebo interni chyba
            free_tree(tree);
            throw_err(syntax_result);
        }

        int semantics_result = check_semantics(tree);

        if (semantics_result != 0) {
            // semanticka chyba nebo interni chyba
            throw_err(semantics_result);
        }

        free_tree(tree);
    }

    return 0;
}
