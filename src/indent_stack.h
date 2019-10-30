#ifndef IFJ_INDENT_STACK_H
#define IFJ_INDENT_STACK_H

#include <stdbool.h>

#define STACK_MAX_SIZE 30

typedef struct {
    unsigned level;
    int data[STACK_MAX_SIZE];
} IndentStack;

void stack_init(IndentStack* is);
void stack_pop(IndentStack* is);
int stack_top(IndentStack* is);
void stack_push(IndentStack* is, int value);
bool stack_empty(IndentStack* is);

#endif //IFJ_INDENT_STACK_H
