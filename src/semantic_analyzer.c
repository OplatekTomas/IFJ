//
// Created by Petr on 26/11/2019.
//

#include "semantic_analyzer.h"

int check_semantics(ASTNode *tree) {
    int result = check_types(tree);
    if (result != 0) {
        return result;
    }
    return 0;
}

int check_types(ASTNode *tree) {
    return 0;
}
