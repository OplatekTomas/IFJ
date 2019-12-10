//
// Created by tomas on 27.11.19.
//

#include "code_gen.h"

void generate_print(ASTNode* tree, SymTable **table);


unsigned int counter = 0;

void generate_func_call(ASTNode* node, SymTable** table) {
    if (strcmp(node->symbol->id, "print") == 0) {
        generate_print(node, table);
        return;
    }
    printf("CREATEFRAME\n");
    for (unsigned i = 0; i < (unsigned)node->subnode_len; i++) {
        printf("DEFVAR TF@%i\n", i);
        printf("MOVE TF@%i ", i);
        switch (node->nodes[i]->node_type) {
            case IDENTIFICATOR:
                // wut
                break;
            case VALUE:
            default:
                switch (node->nodes[i]->arith_type) {
                    case TYPE_NONE:
                        printf("nil@nil\n");
                        break;
                    case TYPE_FLOAT:
                        printf("float@%a\n", node->nodes[i]->n.d);
                        break;
                    case TYPE_INT:
                        printf("int@%d\n", node->nodes[i]->n.i);
                        break;
                    case TYPE_STRING:
                        printf("string@%s\n", node->nodes[i]->str_val);
                        break;
                    default:
                        // wut
                        break;
                }
                break;
        }
    }
    printf("CALL $%s\n", node->symbol->id);
}

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
    }else if(tree->arith_type == TYPE_STRING){
        strcpy(arr, "string@");
        strcat(arr, tree->str_val);
    }else if(tree->arith_type == TYPE_INT){
        strcpy(arr, "int@");
        char tmp[128] = {0};
        sprintf(tmp,"%d",tree->n.i);
        strcat(arr, tmp);
    }else{
        strcpy(arr, "float@");
        char tmp[128] = {0};
        sprintf(tmp,"%a",tree->n.d);
        strcat(arr, tmp);
    }
    return arr;
}

unsigned int generate_exp(ASTNode* tree, SymTable ** table, bool is_global){
    unsigned result = 0;
    if(tree->nodes[0]->node_type == FLOAT_TO_INT){
        tree->nodes[0] = tree->nodes[0]->nodes[0];
        char * tmp = get_expression_arg(tree->nodes[0], table);
        printf("INT2FLOAT %s %s\n", tmp, tmp);
    }else if(tree->nodes[1]->node_type == FLOAT_TO_INT){
        tree->nodes[1] = tree->nodes[1]->nodes[0];
        char * tmp = get_expression_arg(tree->nodes[1], table);
        printf("INT2FLOAT %s %s\n", tmp, tmp);
    }
    if(!(tree->nodes[0]->node_type == IDENTIFICATOR || tree->nodes[0]->node_type == VALUE)){
        result = generate_exp(tree->nodes[0], table, is_global);
        printf("DEFVAR TF@%%%d\n", counter);
        printf("%s TF@%%%d TF@%%%d %s\n", get_expression_instr(tree->node_type), counter, result, get_expression_arg(tree->nodes[1], table));

    } else if (!(tree->nodes[1]->node_type == IDENTIFICATOR || tree->nodes[1]->node_type == VALUE)){
        result = generate_exp(tree->nodes[1], table, is_global);
        printf("DEFVAR TF@%%%d\n", counter);
        printf("%s TF@%%%d %s TF@%%%d\n", get_expression_instr(tree->node_type), counter, get_expression_arg(tree->nodes[0], table), result);

    }else{
        printf("DEFVAR TF@%%%d\n", counter);
        printf("%s TF@%%%d %s %s\n", get_expression_instr(tree->node_type), counter, get_expression_arg(tree->nodes[0], table), get_expression_arg(tree->nodes[1], table));
    }
    counter++;
    //TODO: This entire fucking function
    return counter - 1;
}

unsigned int generate_expression(ASTNode* tree, SymTable ** table, bool is_global) {
    printf("PUSHFRAME\nCREATEFRAME\n");
    unsigned int result = generate_exp(tree, table, is_global);
    return result;
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
                printf("MOVE %s@%s string@%s\n", get_frame(is_global), tb->id, tree->nodes[1]->str_val);
                break;
            case TYPE_INT:
                printf("MOVE %s@%s int@%d\n",get_frame(is_global), tb->id, tree->nodes[1]->n.i);
                break;
            case TYPE_FLOAT:
                printf("MOVE %s@%s float@%a\n", get_frame(is_global), tb->id, tree->nodes[1]->n.d);
                break;
            default:
                break;

        }
    }else if(tree->nodes[1]->node_type == IDENTIFICATOR){
        //TODO FUCK ITS A VARIBALE
    }else if(tree->nodes[1]->node_type == FUNCITON_CALL){
        generate_func_call(tree->nodes[1], table);
        printf("MOVE %s@%s TF@%%retval", get_frame(is_global), tb->id);
    }else{
        int result = generate_expression(tree->nodes[1], table, is_global);
        printf("MOVE %s@%s TF@%%%d\n", is_global ? "GF" : "LF", tb->id, result);
        printf("POPFRAME\n");
    }
}

void generate_if_else(ASTNode* tree, SymTable **table){
    CondType condition = tree->nodes[0]->condType;
    struct node* id1 = tree->nodes[0]->nodes[0];
    struct node* id2 = tree->nodes[0]->nodes[1];
    int a = generate_expression(id1, table);
    int b = generate_expression(id2, table);
    switch(condition){
        case OP_EQ:
            printf("EQ <var> TF@%d TF@%d\n", a, b);
            break;
        case OP_NEQ:
            printf("EQ <var> TF@%d TF@%d\n", a, b);
            printf("NOT <var> <var>\n");
            break;
        case OP_LS:
            printf("LT <var> TF@%d TF@%d\n", a, b);
            break;
        case OP_LSEQ:
            printf("LT <var1> TF@%d TF@%d\n", a, b);
            printf("EQ <var2> TF@%d TF@%d\n", a, b);
            printf("OR <var1> <var2>\n");
            break;
        case OP_GR:
            printf("GT <var> TF@%d TF@%d\n", a, b);
            break;
        case OP_GREQ:
            printf("GT <var> TF@%d TF@%d\n", a, b);
            printf("EQ <var2> TF@%d TF@%d\n", a, b);
            printf("OR <var1> <var2>\n");
            break;
        default:
            break;
    }
    printf("LABEL IF%d\n", counter);
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
                generate_if_else(tree, table);
                break;
            case WHILE_LOOP:
                //generate_while_loop(tree, table);
                break;
            case FUNCTION_DEFINITION:
                generate_definition(tree, table);
                break;
            case FUNCITON_CALL:
                generate_func_call(tree, table);
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
    printf(".IFJcode19\nCREATEFRAME\n");
    handle_next_block(tree, table);

}

void generate_read(char* frame, char* id, char* type){
    printf("READ %s@%s %s\n", frame, id, type);
}
void generate_print(ASTNode* tree, SymTable **table) {
    for (unsigned i = 0; i < tree->subnode_len; i++) {
        switch (tree->nodes[i]->node_type) {
            case IDENTIFICATOR:
                if (is_symbol_global(tree->nodes[i]->symbol, table)) {
                    printf("WRITE GF@%s", tree->nodes[i]->symbol->id);
                } else {
                    printf("WRITE LF@%s", tree->nodes[i]->symbol->id);
                }
                break;
            case VALUE:
            default:
                switch (tree->nodes[i]->arith_type) {
                    case TYPE_NONE:
                        printf("WRITE nil@nil\n");
                        break;
                    case TYPE_FLOAT:
                        printf("WRITE float@%a\n", tree->nodes[i]->n.d);
                        break;
                    case TYPE_INT:
                        printf("WRITE int@%d\n", tree->nodes[i]->n.i);
                        break;
                    case TYPE_STRING:
                        printf("WRITE string@%s\n", tree->nodes[i]->str_val);
                        break;
                    default:
                        // wut
                        break;
                }
                break;
        }
    }
}
