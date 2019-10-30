//
// Created by petr on 30.10.19.
//

#include "syntax_stack.h"

void syntax_stack_push(SyntaxStack* ss, Word w, WordType wt) {
    ss->data[ss->index] = w;
    ss->types[ss->index] = wt;
    ss->index++;
}

void syntax_Stack_init(SyntaxStack* ss) {
    ss->index = 1;
    ss->data[0].n = PROGRAM_END;
}

void syntax_stack_pop(SyntaxStack* ss) {
    if (!syntax_stack_empty(ss)) {
        ss->index--;
    }
}

bool syntax_stack_full(SyntaxStack* ss) {
    return ss->index == SYNTAX_STACK_MAX_SIZE;
}

bool syntax_stack_empty(SyntaxStack* ss) {
    return ss->index == 0;
}

Word syntax_stack_top(SyntaxStack* ss, WordType* wt) {
    *wt = ss->types[ss->index];
    return ss->data[ss->index];
}

