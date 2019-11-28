//
// Created by tomas on 27.11.19.
//

#include "code_gen.h"


void generate_code(ASTNode* tree, SymTable **table, FILE* output){
    int size = 0;
    ASTNode** result = get_preorder(tree, &size);
    printHT(table);
    free(result);
}

