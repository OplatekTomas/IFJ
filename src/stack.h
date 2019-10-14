#ifndef IFJ_STACK_H
#define IFJ_STACK_H

#include <stdbool.h>

#define STACK_MAX_SIZE 30

typedef struct {
    unsigned level;
    int data[STACK_MAX_SIZE];
} indentStack;

void stack_init(indentStack* is);
void stack_pop(indentStack* is);
int stack_top(indentStack* is);
void stack_push(indentStack* is, int value);
bool stack_empty(indentStack* is);

#endif //IFJ_STACK_H
