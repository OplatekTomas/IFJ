#include "stack.h"

void stack_init(IndentStack* is) {
    is->level = 0;
    is->data[0] = 0;
}

void stack_pop(IndentStack* is) {
    if (!stack_empty(is)) {
        is->level--;
    }
}

int stack_top(IndentStack *is) {
    return is->data[is->level];
}

void stack_push(IndentStack* is, int value) {
    is->level++;
    is->data[is->level] = value;
}

bool stack_empty(IndentStack* is) {
    return is->level == 0;
}

