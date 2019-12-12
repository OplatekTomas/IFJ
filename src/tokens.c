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
 * tokens.c
 */

#include "tokens.h"

void token_stack_init(TokenStack* ts) {
    ts->index = 0;
}

void token_stack_push(TokenStack* ts, Token t) {
    ts->index++;
    ts->data[ts->index] = t;
}

void token_stack_pop(TokenStack* ts) {
    if (!token_stack_empty(ts)) {
        ts->index--;
    }
}

bool token_stack_empty(TokenStack* ts) {
    return ts->index == 0;
}

Token token_stack_top(TokenStack* ts) {
    return ts->data[ts->index];
}
