//
// Created by tomas on 09.12.19.
//

#ifndef IFJ_PTR_LIST_H
#define IFJ_PTR_LIST_H
#include <stdlib.h>
#include "error.h"

void** arr;

void initPtrList();

void addPtr(void *ptr);

void changePtr(void *old, void *new);

int getSize();

void freeAllPtr();

#endif //IFJ_PTR_LIST_H

