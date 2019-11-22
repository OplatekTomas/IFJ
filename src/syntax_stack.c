//
// Created by petr on 30.10.19.
//

#include "syntax_stack.h"

void syntax_stack_push(SyntaxStack* ss, SSValue value, Token t) {
    ss->data[ss->index] = value;
    ss->tokens[ss->index] = t;
    ss->index++;
}

void syntax_stack_init(SyntaxStack* ss) {
    ss->index = 1;
    ss->data[0] = SYNTAX_END;
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

SSValue syntax_stack_top(SyntaxStack* ss, Token* t) {
    if (t != NULL) {
        *t = ss->tokens[ss->index - 1];
    }
    return ss->data[ss->index - 1];
}

SSValue syntax_stack_nearest_term(SyntaxStack* ss, Token* t, unsigned* loc) {
    for (unsigned i = ss->index - 1; i >= 0; i--) {
        if (ss->data[i] != SYNTAX_EXPR) {
            if (t != NULL) {
                *t = ss->tokens[i];
            }
            if (loc != NULL) {
                *loc = i;
            }
            return ss->data[i];
        }
    }
}

void syntax_stack_shift(SyntaxStack* ss, unsigned index) {
    for (unsigned i = SYNTAX_STACK_MAX_SIZE - 1; i > index; i--) {
        ss->tokens[i] = ss->tokens[i - 1];
        ss->data[i] = ss->data[i - 1];
    }
    ss->data[index + 1] = SYNTAX_LESSER;
    ss->index++;
}

