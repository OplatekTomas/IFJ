#ifndef IFJ_SYNTAX_ANALYZER_H
#define IFJ_SYNTAX_ANALYZER_H

#include "lex_analyzer.h"
#include "syntax_stack.h"
#include "astnode.h"
// zapoznamkovano kvuli docasnym compile chybam
//#include "symtable.h"

/// Vraci kod chyby, ktery je stejny jako kod chyby kompilatoru
int get_derivation_tree(FILE *source, ASTNode** tree);

#endif //IFJ_SYNTAX_ANALYZER_H
