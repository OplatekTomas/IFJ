#ifndef IFJ_SYNTAX_ANALYZER_H
#define IFJ_SYNTAX_ANALYZER_H

#include "lex_analyzer.h"
#include "syntax_stack.h"
#include "astnode.h"
// zapoznamkovano kvuli docasnym compile chybam
//#include "symtable.h"

ASTNode* get_derivation_tree(FILE *source);
int check_block(ASTNode* tree, Scanner* s);

#endif //IFJ_SYNTAX_ANALYZER_H
