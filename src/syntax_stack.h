//
// Created by petr on 30.10.19.
//

#ifndef IFJ_SYNTAX_STACK_H
#define IFJ_SYNTAX_STACK_H

#include "lex_analyzer.h"

#define SYNTAX_STACK_MAX_SIZE 100

typedef enum {
    SYNTAX_GREATER = '>',
    SYNTAX_EQUAL = '=',
    SYNTAX_LESSER = '<',
    SYNTAX_TERM = 'i',
    SYNTAX_EXPR = 'E',
    SYNTAX_END = '$',
    SYNTAX_EMPTY = ' '
} SSValue;

typedef struct {
    unsigned index;
    SSValue data[SYNTAX_STACK_MAX_SIZE];
    Token tokens[SYNTAX_STACK_MAX_SIZE];
} SyntaxStack;

void syntax_stack_push(SyntaxStack* ss, SSValue value, Token t);
void syntax_stack_init(SyntaxStack* ss);
void syntax_stack_pop(SyntaxStack* ss);
bool syntax_stack_full(SyntaxStack* ss);
bool syntax_stack_empty(SyntaxStack* ss);
SSValue syntax_stack_top(SyntaxStack* ss, Token* t);
SSValue syntax_stack_nearest_term(SyntaxStack* ss, Token* t);


#endif //IFJ_SYNTAX_STACK_H
