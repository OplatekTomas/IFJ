//
// Created by Petr on 26/11/2019.
//

#ifndef IFJ_SEMANTIC_ANALYZER_H
#define IFJ_SEMANTIC_ANALYZER_H

#include "astnode.h"

int check_semantics(ASTNode* tree);
int check_types(ASTNode* tree);
int check_division_by_zero(ASTNode* tree);

#endif //IFJ_SEMANTIC_ANALYZER_H
