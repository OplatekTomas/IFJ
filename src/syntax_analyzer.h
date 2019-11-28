#ifndef IFJ_SYNTAX_ANALYZER_H
#define IFJ_SYNTAX_ANALYZER_H

#include "lex_analyzer.h"
#include "syntax_stack.h"
#include "astnode.h"
#include "symtable.h"

/// Vraci kod chyby, ktery je stejny jako kod chyby kompilatoru
int get_derivation_tree(FILE *source, ASTNode** tree, SymTable*** table);

#endif //IFJ_SYNTAX_ANALYZER_H
