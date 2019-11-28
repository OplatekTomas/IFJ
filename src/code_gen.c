//
// Created by tomas on 27.11.19.
//

#include "code_gen.h"

void generate_code(ASTNode *tree, SymTable **table, FILE *output) {
    ASTNode** result = get_postorder(tree);
    printHT(table);
    free(result);
}

