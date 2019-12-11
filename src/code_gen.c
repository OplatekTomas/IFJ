/*
 * IFJ projekt 2019
 * **************************
 * Tým 82, varianta II
 * **************************
 * Autoři:
 * Zdeněk Kolba (xkolba01)
 * Tomáš Oplatek (xoplat01)
 * David Rubý (xrubyd00)
 * Petr Volf (xvolfp00)
 * **************************
 * code_gen.c
 */

#include "code_gen.h"

void generate_print(ASTNode* tree, SymTable **table);
void generate_while_loop(ASTNode* tree, SymTable** table);
void generate_condition(ASTNode* tree, SymTable** table);
void generate_definition(ASTNode* tree, SymTable** table);
void generate_read(char* frame_id, char* type);
bool handle_next_block(ASTNode* root, SymTable** table, bool is_global);
char* get_expression_arg(ASTNode* tree, SymTable** table);
void generate_return(ASTNode* tree, SymTable** table);
void generate_strlen(ASTNode* node, SymTable ** table,bool hasResult);
void generate_getchar(ASTNode* node1, ASTNode* node2, ASTNode* node3, SymTable** table, bool hasResult);
void generate_int2char(ASTNode* node, SymTable ** table, bool hasResult);
void generate_stri2int(ASTNode* node1, ASTNode* node2, SymTable ** table, bool hasResult);


unsigned int counter = 0;

void generate_func_call(ASTNode* node, SymTable** table, bool is_global) {
    if (strcmp(node->symbol->id, "print") == 0) {
        generate_print(node, table);
        return;
    }else if(strcmp(node->symbol->id, "len") == 0) {
        generate_strlen(node->nodes[0], table, false);
        return;
    }else if(strcmp(node->symbol->id, "substr") == 0) {
        generate_getchar(node->nodes[0],node->nodes[1],node->nodes[2], table, false);
        return;
    }else if(strcmp(node->symbol->id, "ord") == 0) {
        generate_stri2int(node->nodes[0], node->nodes[1],table ,false);
        return;
    }
    else if(strcmp(node->symbol->id, "chr") == 0) {
        generate_int2char(node->nodes[0],table, false);
        return;
    }

    printf("PUSHFRAME\n");
    printf("CREATEFRAME\n");
    for (unsigned i = 0; i < (unsigned)node->subnode_len; i++) {
        printf("DEFVAR TF@_%i\n", i);
        printf("MOVE TF@_%i ", i);
        switch (node->nodes[i]->node_type) {
            case IDENTIFICATOR:
                if (is_symbol_global(node->nodes[i]->symbol, table)) {
                    printf("GF@%s", node->nodes[i]->str_val);
                } else {
                    printf("LF@%s", node->nodes[i]->str_val);
                }
                break;
            case VALUE:
            default:
                switch (node->nodes[i]->arith_type) {
                    case TYPE_NONE:
                        printf("nil@nil");
                        break;
                    case TYPE_FLOAT:
                        printf("float@%a", node->nodes[i]->n.d);
                        break;
                    case TYPE_INT:
                        printf("int@%d", node->nodes[i]->n.i);
                        break;
                    case TYPE_STRING:
                        printf("string@%s", node->nodes[i]->str_val);
                        break;
                    default:
                        // wut
                        break;
                }
                break;
        }
        printf("\n");
    }
    printf("CALL $%s\n", node->symbol->id);
}

static char* get_frame(bool is_global){
    return is_global ? "GF" : "TF";
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
            return "ADDS";
        case SUBTRACTION:
            return "SUBS";
        case MULTIPLICATION:
            return "MULS";
        case DIVISION:
            return "DIVS";
        case INT_DIVISION:
            return "IDIVS";
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

void generate_string_exp(ASTNode* tree, SymTable** table, bool is_global){
    int size = 0;
    ASTNode** arr = get_postorder(tree,&size);
    printf("DEFVAR TF@%%%d\n", counter);
    printf("MOVE TF@%%%d string@\n", counter);
    for(int i = 0; i < size; i++){
        if(arr[i]->node_type != ADDITION){
            printf("CONCAT TF@%%%d TF@%%%d %s\n", counter, counter, get_expression_arg(arr[i], table));;
            //printf("%s\n", arr[i]->symbol->id);
        }
    }
    printf("PUSHS TF@%%%d\n", counter);
    counter++;
}

void generate_exp(ASTNode* tree, SymTable ** table, bool is_global){
    bool shouldConvertFirst = false;
    bool shouldConvertSecond = false;
    if(tree->nodes[0]->node_type == FLOAT_TO_INT){
        shouldConvertFirst = true;
        tree->nodes[0] = tree->nodes[0]->nodes[0];
    }else if(tree->nodes[1]->node_type == FLOAT_TO_INT){
        shouldConvertSecond = true;
        tree->nodes[1] = tree->nodes[1]->nodes[0];
    }
    if(tree->arith_type == TYPE_STRING){
        printf("PUSHFRAME\nCREATEFRAME\n");
        generate_string_exp(tree,table, is_global);
        printf("POPFRAME\n");
    }else if(!(tree->nodes[0]->node_type == IDENTIFICATOR || tree->nodes[0]->node_type == VALUE)){
        generate_exp(tree->nodes[0], table, is_global);
        if(shouldConvertFirst){
            printf("INT2FLOATS\n");
        }
        printf("PUSHS %s\n",get_expression_arg(tree->nodes[1], table));
        if(shouldConvertSecond){
            printf("INT2FLOATS\n");
        }
        printf("%s\n",get_expression_instr(tree->node_type));

    } else if (!(tree->nodes[1]->node_type == IDENTIFICATOR || tree->nodes[1]->node_type == VALUE)){
        generate_exp(tree->nodes[1], table, is_global);
        if(shouldConvertSecond){
            printf("INT2FLOATS\n");
        }
        printf("PUSHS %s\n",get_expression_arg(tree->nodes[0], table));
        if(shouldConvertFirst){
            printf("INT2FLOATS\n");
        }
        printf("%s\n",get_expression_instr(tree->node_type));

    }else{
         printf("PUSHS %s\n",get_expression_arg(tree->nodes[0], table));
         if(shouldConvertFirst){
             printf("INT2FLOATS\n");
         }
         printf("PUSHS %s\n",get_expression_arg(tree->nodes[1], table));
         if(shouldConvertSecond){
             printf("INT2FLOATS\n");
         }
         printf("%s\n",get_expression_instr(tree->node_type));
    }
}

void generate_expression(ASTNode* tree, SymTable ** table, bool is_global) {
    if(tree->node_type == IDENTIFICATOR){
        printf("PUSHS %s\n", get_expression_arg(tree, table));
    }else if(tree->node_type == VALUE){
        printf("PUSHS ");
        switch (tree->arith_type) {
            case TYPE_NONE:
                printf("nil@nil\n");
                break;
            case TYPE_FLOAT:
                printf("float@%a\n", tree->n.d);
                break;
            case TYPE_INT:
                printf("int@%d\n", tree->n.i);
                break;
            case TYPE_STRING:
                printf("string@%s\n", tree->str_val);
                break;
            default:
                // wut
                break;
        }
    }else{
        generate_exp(tree, table, is_global);
    }
}



void generate_assignment(ASTNode* tree, SymTable ** table, bool is_global){
    SymTable* tb =  tree->nodes[0]->symbol;
    generate_variable(tree->nodes[0], is_global);
    if(tree->nodes[1]->node_type == VALUE) {
        switch (tree->nodes[1]->arith_type) {
            case TYPE_STRING:
                printf("MOVE %s@%s string@%s\n", get_frame(is_global), tb->id, tree->nodes[1]->str_val);
                break;
            case TYPE_INT:
                printf("MOVE %s@%s int@%d\n",get_frame(is_global), tb->id, tree->nodes[1]->n.i);
                break;
            case TYPE_FLOAT:
                printf("MOVE %s@%s float@%a\n", get_frame(is_global), tb->id, tree->nodes[1]->n.d);
                break;
            case TYPE_NONE:
                printf("MOVE %s@%s nil@nil\n", get_frame(is_global), tb->id);
                break;
            default:
                break;

        }
    }else if(tree->nodes[1]->node_type == IDENTIFICATOR){
        printf("MOVE %s %s\n",get_expression_arg(tree->nodes[0], table), get_expression_arg(tree->nodes[1], table));
    }else if(tree->nodes[1]->node_type == FUNCITON_CALL){
        if(strcmp(tree->nodes[1]->symbol->id, "inputi") == 0) {
            generate_read(get_expression_arg(tree->nodes[0], table), "int");
            return;
        }
        else if(strcmp(tree->nodes[1]->symbol->id, "inputf") == 0) {
            generate_read(get_expression_arg(tree->nodes[0], table), "float");
            return;
        }
        else if(strcmp(tree->nodes[1]->symbol->id, "inputs") == 0) {
            generate_read(get_expression_arg(tree->nodes[0], table), "string");
            return;
        }
        else if(strcmp(tree->nodes[1]->symbol->id, "len") == 0) {
            generate_strlen(tree->nodes[1]->nodes[0], table, true);
            return;
        }
        else if(strcmp(tree->nodes[1]->symbol->id, "substr") == 0) {
            generate_getchar(tree->nodes[1]->nodes[0],tree->nodes[1]->nodes[1],tree->nodes[1]->nodes[2], table, true);
            return;
        }
        else if(strcmp(tree->nodes[1]->symbol->id, "ord") == 0) {
            generate_stri2int(tree->nodes[1]->nodes[0],tree->nodes[1]->nodes[1], table,true);
            return;
        }
        else if(strcmp(tree->nodes[1]->symbol->id, "chr") == 0) {
            generate_int2char(tree->nodes[1]->nodes[0],table,true);
            return;
        }
        generate_func_call(tree->nodes[1], table, is_global);
        printf("MOVE %s@%s TF@%%retval\n", get_frame(is_global), tb->id);
    }else{
        generate_expression(tree->nodes[1], table, is_global);
        printf("POPS %s@%s\n", is_global ? "GF" : "TF", tb->id);
        counter++;
    }
}


SymTable ** get_vars_defined_in_block(ASTNode* tree, int* size){
    *size = 0;
    int orderSize = 0;
    int maxSize = 256;
    SymTable** res = malloc(sizeof(SymTable*)*maxSize);
    ASTNode** post = get_postorder(tree, &orderSize);
    for(int i = 0; i < orderSize; i++){
        if(*size >= maxSize){
            maxSize += 256;
            res = realloc(res, sizeof(SymTable*)*maxSize);
        }
        if(post[i]->node_type == ASSIGNMENT && !post[i]->nodes[0]->symbol->has_been_defined){
            res[*size] = post[i]->nodes[0]->symbol;
            (*size)++;
        }
    }
    addPtr(res);
    return res;
}
void undefine_vars_from_block(SymTable** items, int size){
    for(int i = 0; i < size; i++){
        items[i]->has_been_defined = false;
    }
}

void generate_if_else(ASTNode* tree, SymTable **table, bool is_global){
    generate_condition(tree->nodes[0], table);
    //counter++;
    unsigned tmpCnt = counter;
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS $IFFALSE$%d\n", tmpCnt);
    counter++;
    int size = 0;
    SymTable** arr = get_vars_defined_in_block(tree->nodes[1], &size);
    handle_next_block(tree->nodes[1],table,is_global);
    undefine_vars_from_block(arr, size);
    printf("JUMP $IFEND$%d\n", tmpCnt);
    printf("LABEL $IFFALSE$%d\n", tmpCnt);
    arr = get_vars_defined_in_block(tree->nodes[2], &size);
    undefine_vars_from_block(arr, size);
    handle_next_block(tree->nodes[2],table,is_global);
    printf("LABEL $IFEND$%d\n", tmpCnt);
}

bool handle_next_block(ASTNode* root, SymTable** table, bool is_global){
    int found_return = false;
    ASTNode* tree;
    for(unsigned i = 0; i < root->subnode_len; i++){
        tree = root->nodes[i];
        switch(tree->node_type){
            case EXPRESSION:
                generate_expression(tree, table, is_global);
                break;
            case IF_ELSE:
                generate_if_else(tree, table, is_global);
                break;
            case WHILE_LOOP:
                generate_while_loop(tree, table);
                break;
            case FUNCITON_CALL:
                generate_func_call(tree, table, is_global);
                break;
            case ASSIGNMENT:
                generate_assignment(tree, table, is_global);
                break;
            case RETURN_VALUE:
                generate_return(tree->nodes[0], table);
                found_return = true;
            default:
                break;
        }
    }
    return found_return;
}

void generate_definition(ASTNode* tree, SymTable** table){
    printf("LABEL $%s\n", tree->symbol->id);
    printf("DEFVAR LF@%%retval\n");
    printf("MOVE LF@%%retval nil@nil\n");
    Arguments* current_arg = tree->symbol->args;
    for (int i = 0; i < tree->symbol->argNum; i++) {
        printf("DEFVAR TF@%s\n", current_arg->id);
        printf("MOVE TF@%s TF@_%d\n", current_arg->id, i);
        current_arg = current_arg->nextArg;
    }
    if (!handle_next_block(tree->nodes[0], table, false)) {
        printf("POPFRAME\n");
        printf("RETURN\n");
    }
}

void generate_code(ASTNode* tree, SymTable **table) {
    printHT(table);
    printf(".IFJcode19\nCREATEFRAME\n");
    handle_next_block(tree, table, true);
    printf("EXIT int@0\n");

    // generate definitions
    int size = 0;
    ASTNode** nodes = get_preorder(tree, &size);
    for (int i = 0; i < size; i++) {
        if (nodes[i]->node_type != FUNCTION_DEFINITION) {
            continue;
        }
        generate_definition(nodes[i], table);
    }
}

void generate_read(char* frame_id, char* type){
    printf("READ %s %s\n", frame_id, type);
}

void generate_strlen(ASTNode* node, SymTable** table, bool hasResult){
    printf("DEFVAR TF@%%%d\n", counter);
    printf("TYPE TF@%%%d %s\n",counter , get_expression_arg(node, table));
    printf("JUMPIFEQ $TYPECHECK$%d TF@%%%d string@string\n", counter, counter);
    printf("EXIT int@4\n");
    printf("LABEL $TYPECHECK$%d\n", counter);
    counter++;
    if(hasResult){
        printf("STRLEN LF@%%retval %s\n", get_expression_arg(node, table));
    }
}

void generate_getchar(ASTNode* node1, ASTNode* node2, ASTNode* node3, SymTable** table, bool hasResult){
    unsigned tmpCnt = counter;
    printf("DEFVAR TF@%%%d\n", tmpCnt);

    printf("TYPE TF@%%%d %s\n", tmpCnt, get_expression_arg(node1, table)); //check param 1
    printf("JUMPIFEQ $TYPECHECK$%d TF@%%%d string@string\n", counter, tmpCnt);
    printf("EXIT int@4\n");
    printf("LABEL $TYPECHECK$%d\n", counter);
    counter++;
    printf("TYPE TF@%%%d %s\n", tmpCnt, get_expression_arg(node2, table)); //Check param 2
    printf("JUMPIFEQ $TYPECHECK$%d TF@%%%d string@int\n", counter, tmpCnt);
    printf("EXIT int@4\n");
    printf("LABEL $TYPECHECK$%d\n", counter);
    counter++;
    printf("TYPE TF@%%%d %s\n", tmpCnt, get_expression_arg(node3, table)); //check param 3
    printf("JUMPIFEQ $TYPECHECK$%d TF@%%%d string@int\n", counter, tmpCnt);
    printf("EXIT int@4\n");
    printf("LABEL $TYPECHECK$%d\n", counter);
    counter++;
    if(hasResult){
        for(int i = node2->n.i; i < node2->n.i + node3->n.i; i++){
            printf("GETCHAR LF@%%retval %s %d\n", get_expression_arg(node1, table), i);
        }
    }
}

void generate_stri2int(ASTNode* node1, ASTNode* node2, SymTable ** table, bool hasResult){
    unsigned tmpCnt = counter;
    printf("DEFVAR TF@%%%d\n", counter);
    printf("TYPE TF@%%%d %s\n", tmpCnt, get_expression_arg(node1, table)); //check param 1
    printf("JUMPIFEQ $TYPECHECK$%d TF@%%%d string@string\n", counter, tmpCnt);
    printf("EXIT int@4\n");
    printf("LABEL $TYPECHECK$%d\n", counter);
    counter++;
    printf("TYPE TF@%%%d %s\n", tmpCnt, get_expression_arg(node2, table)); //Check param 2
    printf("JUMPIFEQ $TYPECHECK$%d TF@%%%d string@int\n", counter, tmpCnt);
    printf("EXIT int@4\n");
    printf("LABEL $TYPECHECK$%d\n", counter);
    counter++;
    if(hasResult){
        printf("STRI2INT LF@%%retval %s %s\n", get_expression_arg(node1, table), get_expression_arg(node2, table));
    }
}

void generate_int2char(ASTNode* node, SymTable ** table, bool hasResult) {
    printf("DEFVAR TF@%%%d\n", counter);
    printf("TYPE TF@%%%d %s\n", counter, get_expression_arg(node, table)); //check param 1
    printf("JUMPIFEQ $TYPECHECK$%d TF@%%%d string@int\n", counter, counter);
    printf("EXIT int@4\n");
    printf("LABEL $TYPECHECK$%d\n", counter);
    counter++;
    if(hasResult){
        printf("INT2CHAR LF@%%retvar %s\n", get_expression_arg(node, table));
    }
}


void generate_print(ASTNode* tree, SymTable **table) {
    for (unsigned i = 0; i < tree->subnode_len; i++) {
        switch (tree->nodes[i]->node_type) {
            case IDENTIFICATOR:
                if (is_symbol_global(tree->nodes[i]->symbol, table)) {
                    printf("WRITE GF@%s\n", tree->nodes[i]->symbol->id);
                } else {
                    printf("WRITE TF@%s\n", tree->nodes[i]->symbol->id);
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

void generate_while_loop(ASTNode* tree, SymTable** table) {
    int loop_index = counter;
    counter++;

    // definice promennych pred loopem

    int size = 0;
    ASTNode** nodes = get_postorder(tree->nodes[1], &size);
    int new_assign_count = 0;
    for(int i = 0; i < size; i++) {
        if (nodes[i]->node_type == ASSIGNMENT && !nodes[i]->nodes[0]->symbol->has_been_defined) {
            new_assign_count++;
        }
    }

    if (new_assign_count > 0) {
        //printf("PUSHFRAME\n");
        //printf("CREATEFRAME\n");
        for(int i = 0; i < size; i++) {
            if (nodes[i]->node_type == ASSIGNMENT && !nodes[i]->nodes[0]->symbol->has_been_defined) {
                printf("DEFVAR %s\n" ,get_expression_arg(nodes[i]->nodes[0], table));
                nodes[i]->nodes[0]->symbol->has_been_defined = true;
            }
        }
    }

    // podminka
    printf("LABEL $while$%d\n", loop_index);
    generate_condition(tree->nodes[0], table);
    printf("PUSHS bool@false\n");

    printf("JUMPIFEQS $while_end$%d\n", loop_index);

    // loop
    //printf("PUSHFRAME\n");
    //printf("CREATEFRAME\n");
    handle_next_block(tree->nodes[1], table, true);
    //printf("POPFRAME\n");

    // konec loopu, skok na podminku
    printf("JUMP $while$%d\n", loop_index);

    // pokracovani
    printf("LABEL $while_end$%d\n", loop_index);
    if (new_assign_count > 0) {
        //printf("POPFRAME\n");
    }
}

void generate_condition(ASTNode* tree, SymTable** table) {
    if(tree->subnode_len == 1) {
        unsigned type_counter = counter;
        counter++;
        printf("DEFVAR TF@%%%d\n", type_counter);
        printf("TYPE TF@%%%d %s\n", type_counter, get_expression_arg(tree->nodes[0], table));
        printf("PUSHS TF@%%%d\n", type_counter);
        printf("PUSHS string@int\n");
        printf("JUMPIFEQS $$dyntypecheck_int$%d\n", type_counter);
        printf("PUSHS TF@%%%d\n", type_counter);
        printf("PUSHS string@float\n");
        printf("JUMPIFEQS $$dyntypecheck_floatint$%d\n", type_counter);
        printf("PUSHS TF@%%%d\n", type_counter);
        printf("PUSHS string@string\n");
        printf("JUMPIFEQS $$dyntypecheck_string$%d\n", type_counter);
        printf("JUMP $$dyntypecheck_false$%d\n", type_counter);

        printf("LABEL $$dyntypecheck_int$%d\n", type_counter);
        printf("PUSHS %s\n", get_expression_arg(tree->nodes[0], table));
        printf("PUSHS int@0\n");
        printf("JUMPIFNEQS $$dyntypecheck_true$%d\n", type_counter);
        printf("JUMP $$dyntypecheck_false$%d\n", type_counter);

        printf("LABEL $$dyntypecheck_float$%d\n", type_counter);
        printf("PUSHS %s\n", get_expression_arg(tree->nodes[0], table));
        printf("PUSHS float@%a\n", 0.0);
        printf("JUMPIFNEQS $$dyntypecheck_true$%d\n", type_counter);
        printf("JUMP $$dyntypecheck_false$%d\n", type_counter);

        printf("LABEL $$dyntypecheck_string$%d\n", type_counter);
        printf("STRLEN TF@%%%d %s\n", type_counter, get_expression_arg(tree->nodes[0], table));
        printf("PUSHS TF@%%%d\n", type_counter);
        printf("PUSHS int@0\n");
        printf("JUMPIFNEQS $$dyntypecheck_true$%d\n", type_counter);

        printf("LABEL $$dyntypecheck_true$%d\n", type_counter);
        printf("PUSHS bool@true\n");
        printf("JUMP $$dyntypecheck_end$%d\n", type_counter);

        printf("LABEL $$dyntypecheck_false$%d\n", type_counter);
        printf("PUSHS bool@false\n");

        printf("LABEL $$dyntypecheck_end$%d\n", type_counter);
    } else {
        generate_expression(tree->nodes[0], table, false);
        generate_expression(tree->nodes[1], table, false);
        switch(tree->condType){
            case OP_EQ:
                printf("EQS\n");
                break;
            case OP_NEQ:
                printf("EQS\n");
                printf("NOTS\n");
                break;
            case OP_LS:
                printf("LTS\n");
                break;
            case OP_LSEQ:
                printf("LTS\n");
                generate_expression(tree->nodes[0], table, false);
                generate_expression(tree->nodes[1], table, false);
                printf("EQS\n");
                printf("ORS\n");
                break;
            case OP_GR:
                printf("GTS\n");
                break;
            case OP_GREQ:
            default:
                printf("GTS\n");
                generate_expression(tree->nodes[0], table, false);
                generate_expression(tree->nodes[1], table, false);
                printf("EQS\n");
                printf("ORS\n");
                break;
        }
    }
}

void generate_return(ASTNode* tree, SymTable** table) {
    if (tree->node_type == VALUE) {
        switch (tree->arith_type) {
            case TYPE_NONE:
                printf("MOVE LF@%%retval nil@nil\n");
                break;
            case TYPE_FLOAT:
                printf("MOVE LF@%%retval float@%a\n", tree->n.d);
                break;
            case TYPE_INT:
                printf("MOVE LF@%%retval int@%d\n", tree->n.i);
                break;
            case TYPE_STRING:
                printf("MOVE LF@%%retval string@%s\n", tree->str_val);
                break;
            default:
                // wut
                break;
        }
        return;
    } else {
        printf("MOVE LF@%%retval TF@%s\n", tree->symbol->id);
    }
    printf("POPFRAME\n");
    printf("RETURN\n");
}
