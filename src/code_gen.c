//
// Created by tomas on 27.11.19.
//

#include "code_gen.h"

void handle_next_block(ASTNode* tree, SymTable** table){
    switch(tree->node_type){
        case EXPRESSION:
            generate_expression(tree, table);
            break;
        case IF_ELSE:
            generate_if_else(tree, table);
            break;
        case WHILE_LOOP:
            generate_while_loop(tree, table);
            break;
        case FUNCTION_DEFINITION:
            generate_definition(tree, table);
            break;
        case FUNCITON_CALL:
            generate_func_call(tree, table);
            break;
    }
}

void generate_def(ASTNode* tree, SymTable** table){
    printf("LABEL $%s\n", tree->symbol->id);
    printf("PUSHFRAME\n");


    //handle_next_block(tree, table);

    printf("POPFRAME\n");
    printf("RETURN\n");
}


void generate_code(ASTNode* tree, SymTable **table, FILE* output){

    int size = 0;
    //ASTNode** result = get_preorder(tree, &size);
    //printHT(table);
    printf(".IFJcode19\n");
}

