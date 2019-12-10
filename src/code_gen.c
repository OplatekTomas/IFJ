//
// Created by tomas on 27.11.19.
//

#include "code_gen.h"


unsigned int counter = 0;



static char* get_frame(bool is_global){
    return is_global ? "GF" : "LF";
}

void generate_variable(ASTNode* tree, bool is_global){
    if(tree->symbol->has_been_defined){
       return;
    }
    printf("DEFVAR %s@%s\n",get_frame(is_global) ,tree->symbol->id);
    tree->symbol->has_been_defined = true;
}

static char* get_expression_instr(NonTerm term){
    switch(term){
        case ADDITION:
            return "ADD";
        case SUBTRACTION:
            return "SUB";
        case MULTIPLICATION:
            return "MUL";
        case DIVISION:
            return "DIV";
        case INT_DIVISION:
            return "IDIV";
        default:
            return NULL;
    }
}

char* get_expression_arg(ASTNode* tree, SymTable** table){
    char* arr = malloc(sizeof(char)* 256);
    addPtr(arr);
    if(tree->node_type == IDENTIFICATOR){
        strcpy(arr, get_frame(is_symbol_global(tree->symbol, table)));
        strcat(arr, "@");
        strcat(arr, tree->symbol->id);
    }
    return arr;
}

unsigned int generate_expression(ASTNode* tree, SymTable ** table, bool is_global){
    unsigned result = 0;
    if(!(tree->nodes[0]->node_type == IDENTIFICATOR || tree->nodes[0]->node_type == VALUE)){
        result = generate_expression(tree->nodes[0], table, is_global);
        printf("%s %d\n", get_expression_instr(tree->node_type), result);

    } else if (!(tree->nodes[1]->node_type == IDENTIFICATOR || tree->nodes[1]->node_type == VALUE)){
        result = generate_expression(tree->nodes[1], table, is_global);
        printf("%s %d\n", get_expression_instr(tree->node_type), result);

    }else{
        printf("%s TF@%%%d %s\n", get_expression_instr(tree->node_type), counter, get_expression_arg(tree->nodes[0], table));
    }
    counter++;
    //TODO: This entire fucking function
    return counter - 1;
}

void generate_assignment(ASTNode* tree, SymTable ** table, bool is_global){
    SymTable* tb =  tree->nodes[0]->symbol;
    generate_variable(tree->nodes[0], is_global);
    if(tree->nodes[1]->node_type == VALUE) {
        switch (tree->nodes[1]->arith_type) {
            case TYPE_FUNCTION:
                //TODO: FUCK THIS
                break;
            case TYPE_STRING:
                printf("MOVE %s@%s string@\"%s\"\n", get_frame(is_global), tb->id, tree->nodes[1]->str_val);
                break;
            case TYPE_INT:
                printf("MOVE %s@%s int@%d\n",get_frame(is_global), tb->id, tree->nodes[1]->n.i);
                break;
            case TYPE_FLOAT:
                printf("MOVE %s@%s float@%f\n", get_frame(is_global), tb->id, tree->nodes[1]->n.d);
                break;
            default:
                break;

        }
    }else if(tree->nodes[1]->node_type == IDENTIFICATOR){
        //TODO FUCK ITS A VARIBALE
    }else if(tree->nodes[1]->node_type == FUNCITON_CALL){
        //TODO: Add function call
    }else{
        int result = generate_expression(tree->nodes[1], table, is_global);
        printf("MOVE %s@%s tf@%%%d\n", is_global ? "GF" : "LF", tb->id, result);
    }
}

void handle_next_block(ASTNode* root, SymTable** table){
    ASTNode* tree;
    for(unsigned i = 0; i < root->subnode_len; i++){
        tree = root->nodes[i];
        switch(tree->node_type){
            case EXPRESSION:
                generate_expression(tree, table, true);
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
            case ASSIGNMENT:
                generate_assignment(tree, table, true);
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
