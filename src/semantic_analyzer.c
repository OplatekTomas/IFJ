//
// Created by Petr on 26/11/2019.
//

#include "semantic_analyzer.h"

int check_semantics(ASTNode *tree, SymTable** st) {
    int result = check_types(tree);
    if (result != 0) {
        return result;
    }
    result = check_fn_call(tree, st);
    if(result != 0){
        return result;
    }
    return 0;
}

int check_types(ASTNode *tree) {
    unsigned exprs_len = 0;
    ASTNode** exprs = get_all_expressions(tree, &exprs_len);

    printf("nalezeno %u vyrazu\n", exprs_len);

    for (unsigned i = 0; i < exprs_len; i++) {
        ASTNode* expr = exprs[i];
    }

    free(exprs);
    return 0;
}

int check_fn_call(ASTNode * tree, SymTable** st){
    int size = 0;
    ASTNode** arr = get_preorder(tree, &size);

}
