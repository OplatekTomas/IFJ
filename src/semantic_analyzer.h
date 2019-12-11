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
 * semantic_analyzer.h
 */

#ifndef IFJ_SEMANTIC_ANALYZER_H
#define IFJ_SEMANTIC_ANALYZER_H

#include "astnode.h"
#include "symtable.h"

int check_semantics(ASTNode* tree, SymTable** st);
int check_types(ASTNode* tree);
int check_fn_calls(ASTNode * tree, SymTable** st);
int check_division_by_zero(ASTNode* tree);

#endif //IFJ_SEMANTIC_ANALYZER_H
