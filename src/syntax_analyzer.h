#ifndef IFJ_SYNTAX_ANALYZER_H
#define IFJ_SYNTAX_ANALYZER_H

#include "lex_analyzer.h"

void get_derivation_tree(FILE *source);

typedef enum {
    PROGRAM_END,
    STATEMENT,
    EXPRESSION,
    ASSIGNMENT,
    CONDITION,
} NonTerm;

typedef union {
    Token t;
    NonTerm n;
} Word;

typedef enum {
    TOKEN,
    NONTERM
} WordType;

#endif //IFJ_SYNTAX_ANALYZER_H
