#include "stack.h"

void stack_init(indentStack* is) {
    is->level = 0;
    is->data[0] = 0;
}

void stack_pop(indentStack* is) {
    if (!stack_empty(is)) {
        is->level--;
    }
}

int stack_top(indentStack *is) {
    return is->data[is->level];
}

void stack_push(indentStack* is, int value) {
    is->level++;
    is->data[is->level] = value;
}

bool stack_empty(indentStack* is) {
    return is->level == 0;
}

