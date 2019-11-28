//
// Created by Petr on 26/11/2019.
//

#ifndef IFJ_SEMANTIC_ANALYZER_H
#define IFJ_SEMANTIC_ANALYZER_H

#include "astnode.h"
#include "symtable.h"

int check_semantics(ASTNode* tree, SymTable** st);
int check_types(ASTNode* tree);
int check_fn_calls(ASTNode * tree, SymTable** st);

#endif //IFJ_SEMANTIC_ANALYZER_H
