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
 * ptr_list.h
 */

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

