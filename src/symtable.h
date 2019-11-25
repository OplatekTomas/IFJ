//
// Created by kolba on 19.11.2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef enum {
    NONE,
    INT,
    FLOAT,
    STRING
} typeValue;

#define SIZE 97

#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

typedef struct symTable{
    char* id;
    int type;
    void* dataPtr;
    int argNum;
    int* arguments;
    int scope;
    struct symTableFunc* ptrNext;
};

struct symTable* hashTable[SIZE];

unsigned int htabHashFunction(const char *str);

void deleteST(char* id);

void freeHT();

void insertST(struct symTable* ptr);

struct symTable* searchST(char* id);

#endif //IFJ_SYMTABLE_H
