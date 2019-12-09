//
// Created by tomas on 27.11.19.
//

#include "code_gen.h"


void generate_code(ASTNode* tree, SymTable **table, FILE* output){

    int size = 0;
    //ASTNode** result = get_preorder(tree, &size);
    //printHT(table);
    printf(".IFJcode19\n");
}

void generate_print(ASTNode* tree, SymTable **table, FILE* output) {
    for (unsigned i = 0; i < tree->subnode_len; i++) {
        printf("WRITE LF@%s", tree->nodes[i]->symbol->id);
    }
}
