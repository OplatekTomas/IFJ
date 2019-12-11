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
 * syntax_analyzer.h
 */

#ifndef IFJ_SYNTAX_ANALYZER_H
#define IFJ_SYNTAX_ANALYZER_H

#include "lex_analyzer.h"
#include "syntax_stack.h"
#include "astnode.h"
#include "symtable.h"

/// Vraci kod chyby, ktery je stejny jako kod chyby kompilatoru
int get_derivation_tree(FILE *source, ASTNode** tree, SymTable*** table);

#endif //IFJ_SYNTAX_ANALYZER_H
