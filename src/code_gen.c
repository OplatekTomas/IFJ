//
// Created by tomas on 27.11.19.
//

#include "code_gen.h"

void handle_next_block(ASTNode* root, SymTable** table){

    ASTNode* tree;
    for(unsigned i = 0; i < root->subnode_len; i++){
        tree = root->nodes[i];
        switch(tree->node_type){
            case EXPRESSION:
                //generate_expression(tree, table);
                break;
            case IF_ELSE:
                //generate_if_else(tree, table);
                break;
            case WHILE_LOOP:
                //generate_while_loop(tree, table);
                break;
            case FUNCTION_DEFINITION:
                generate_definition(tree, table);
                break;
            case FUNCITON_CALL:
                //generate_func_call(tree, table);
                break;
            default:
                break;
        }
    }
}

void generate_definition(ASTNode* tree, SymTable** table){
    printf("told ya\n");
    printf("LABEL $%s\n", tree->symbol->id);
    printf("PUSHFRAME\n");
    handle_next_block(tree->nodes[0], table);
    printf("POPFRAME\n");
    printf("RETURN\n");
}


void generate_code(ASTNode* tree, SymTable **table, FILE* output){

    int size = 0;
    //ASTNode** result = get_preorder(tree, &size);
    //printHT(table);
    printf(".IFJcode19\n");

    handle_next_block(tree, table);
}

void generate_print(ASTNode* tree, SymTable **table, FILE* output) {
    for (unsigned i = 0; i < tree->subnode_len; i++) {
        printf("WRITE LF@%s", tree->nodes[i]->symbol->id);
    }
}
