#include <stdio.h>
#include "syntax_analyzer.h"
#include "semantic_analyzer.h"
#include "code_gen.h"
#include "error.h"

int main (int argc, char *argv[]) {
#ifndef NDEBUG
    if (argc == 1) {
        fprintf(stderr, "neni predany zdrojovy parametr\n");
        throw_err(INTERN_ERR);
    }

    FILE *f = fopen(argv[1], "r");

    if (f == NULL) {
        fprintf(stderr, "neexistujici soubor\n");
        throw_err(INTERN_ERR);
    }
#endif

    ASTNode* tree;

#ifndef NDEBUG
    int syntax_result = get_derivation_tree(f, &tree);
#else
    int syntax_result = get_derivation_tree(stdin, &tree);
#endif

    if (syntax_result != 0) {
        // lexikalni, syntakticka nebo interni chyba
        #ifndef NDEBUG
        fclose(f);
        #endif
        throw_err(syntax_result);
    }
    print_tree(tree);
    int semantics_result = check_semantics(tree);

    if (semantics_result != 0) {
        // semanticka chyba nebo interni chyba
#ifndef NDEBUG
        fclose(f);
#endif
        free_tree(tree);
        throw_err(semantics_result);
    }
    GenerateCode(tree, NULL, stdout);
    free_tree(tree);
#ifndef NDEBUG
    fclose(f);
#endif
    return 0;
}
