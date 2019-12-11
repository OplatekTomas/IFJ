#include <stdio.h>
#include "syntax_analyzer.h"
#include "semantic_analyzer.h"
#include "code_gen.h"
#include "error.h"
#include "symtable.h"

int main (int argc, char *argv[]) {
    initPtrList();

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

    ASTNode* tree = NULL;
    SymTable** table = NULL;

#ifndef NDEBUG
    int syntax_result = get_derivation_tree(f, &tree, &table);
#else
    int syntax_result = get_derivation_tree(stdin, &tree, &table);
#endif
    if (syntax_result != 0) {
        freeAllPtr();

        // lexikalni, syntakticka nebo interni chyba
        #ifndef NDEBUG
        fclose(f);
        #endif
        throw_err(syntax_result);
    }
    print_tree(tree);
    int semantics_result = check_semantics(tree, table);

    if (semantics_result != 0) {
        // semanticka chyba nebo interni chyba
#ifndef NDEBUG
        fclose(f);
#endif
        freeAllPtr();
        //free_tree(tree);
        //printHT(table);
        //freeHT(table);
        throw_err(semantics_result);
    }

    generate_code(tree, table);
    freeAllPtr();
    //free_tree(tree);
    //freeHT(table);
#ifndef NDEBUG
    fclose(f);
#endif
    return 0;
}
