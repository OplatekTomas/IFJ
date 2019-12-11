//
// Created by Petr on 26/11/2019.
//

#include "semantic_analyzer.h"

int check_fn_call(ASTNode * tree, SymTable** st);

int check_division_by_zero(ASTNode* tree) {
    int size = 0;
    ASTNode** table = get_postorder(tree, &size);
    for(int i = 0; i < size; i++){
        if(table[i]->node_type == DIVISION || table[i]->node_type == INT_DIVISION){
            if(table[i]->nodes[1]->arith_type == TYPE_INT){
                if(table[i]->nodes[1]->n.i == 0){
                    return 9;
                }
            }
            else if(table[i]->nodes[1]->arith_type == TYPE_FLOAT){
                if(table[i]->nodes[1]->n.d == 0.0){
                    return 9;
                }
            }else if(table[i]->nodes[1]->node_type == FLOAT_TO_INT){
                if(table[i]->nodes[1]->nodes[0]->n.i == 0){
                    return 9;
                }
            }
        }
    }
    return 0;
}

int check_semantics(ASTNode *tree, SymTable** st) {
    int result = check_types(tree);
    if (result != 0) {
        return result;
    }
    result = check_fn_call(tree, st);
    if(result != 0){
        return result;
    }
    result = check_division_by_zero(tree);
    if(result != 0){
        return result;
    }
    return 0;
}

int check_types(ASTNode *tree) {
    unsigned exprs_len = 0;
    ASTNode** exprs = get_all_expressions(tree, &exprs_len);

    fprintf(stderr,"nalezeno %u vyrazu\n", exprs_len);

    for (unsigned i = 0; i < exprs_len; i++) {
        ASTNode* expr = exprs[i];
    }

    return 0;
}

int check_fn_call(ASTNode * tree, SymTable** st){
    int size = 0;
    ASTNode** arr = get_preorder(tree, &size);
    printHT(st);
    for(int i = 0; i < size; i++){
        ASTNode * test = arr[i];
        if(arr[i]->node_type != FUNCITON_CALL){
            continue;
        }
        SymTable* t = searchST(st, arr[i]->str_val, NULL);
        test->symbol = t;
        //free(test->str_val);
        if(t == NULL){
            //free(arr);
            return 3;
        }
        if(t->type != TYPE_FUNCTION){
            return 3;
        }

        if((unsigned)t->argNum != test->subnode_len && t->argNum != -1){
            //free(arr);
            return 5;
        }
    }
    //free(arr);
    return 0;
}
