//
// Created by tomas on 27.11.19.
//

#include "code_gen.h"


unsigned int counter = 0;

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
    printf("LABEL $%s\n", tree->symbol->id);
    printf("PUSHFRAME\n");
    handle_next_block(tree->nodes[0], table);
    printf("POPFRAME\n");
    printf("RETURN\n");
}


void generate_mov(void* to, char* from, char from_type, bool is_global){
    printf("MOVE ");

}

void generate_variable(ASTNode* tree, bool is_global){
    if(tree->symbol->has_been_defined){
       return;
    }
    printf("DEFVAR ");
    if(is_global){
        printf("GF");
    }else{
        printf("LF");
    }
    printf("@%s\n", tree->symbol->id);
    tree->symbol->has_been_defined = true;
}

void generate_expression(ASTNode* tree, SymTable ** table){
    //TODO: This entire fucking function
}

void generate_assignment(ASTNode* tree, SymTable ** table, bool is_global){
    SymTable* tb =  tree->nodes[0]->symbol;
    generate_variable(tree->nodes[0], is_global);
    if(tree->nodes[1]->node_type == VALUE){
        switch(tree->nodes[1]->arith_type){
            case TYPE_FUNCTION:
                //TODO: FUCK THIS
                break;
            case TYPE_STRING:
                printf("MOVE %s@%s \"%s\"", is_global ? "GF" : "LF", tb->id, tree->nodes[1]->str_val);
                break;
            case TYPE_INT:
                printf("MOVE %s@%s %d", is_global ? "GF" : "LF", tb->id, tree->nodes[1]->n.i);
                break;
            case TYPE_FLOAT:
                printf("MOVE %s@%s %f", is_global ? "GF" : "LF", tb->id, tree->nodes[1]->n.d);
                break;
            default:
                break;

        }
    }
    if(tree->nodes[1]->node_type == IDENTIFICATOR){
        //TODO FUCK
    }else if(tree->nodes[1]->node_type == FUNCITON_CALL){
        //TODO: Add function call
    }else{
        generate_expression(tree, table);
    }
}

void generate_code(ASTNode* tree, SymTable **table, FILE* output){

    int size = 0;
    //ASTNode** result = get_preorder(tree, &size);
    printHT(table);
    printf(".IFJcode19\n");
    handle_next_block(tree, table);

}

void generate_read(char* frame, char* id, char* type){
    printf("READ %s@%s %s\n", frame, id, type);
}
void generate_print(ASTNode* tree, SymTable **table, FILE* output) {
    for (unsigned i = 0; i < tree->subnode_len; i++) {
        if (is_symbol_global(tree->nodes[i]->symbol, table)) {
            printf("WRITE GF@%s", tree->nodes[i]->symbol->id);
        } else {
            printf("WRITE LF@%s", tree->nodes[i]->symbol->id);
        }
    }
}
