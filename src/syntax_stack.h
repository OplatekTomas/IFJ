//
// Created by petr on 30.10.19.
//

#ifndef IFJ_SYNTAX_STACK_H
#define IFJ_SYNTAX_STACK_H

#include "syntax_analyzer.h"

#define SYNTAX_STACK_MAX_SIZE 100

typedef struct {
    unsigned index;
    Word data[SYNTAX_STACK_MAX_SIZE];
    WordType types[SYNTAX_STACK_MAX_SIZE];
} SyntaxStack;

void syntax_stack_push(SyntaxStack* ss, Word w, WordType wt);
void syntax_Stack_init(SyntaxStack* ss);
void syntax_stack_pop(SyntaxStack* ss);
bool syntax_stack_full(SyntaxStack* ss);
bool syntax_stack_empty(SyntaxStack* ss);
Word syntax_stack_top(SyntaxStack* ss, WordType* wt);


#endif //IFJ_SYNTAX_STACK_H
