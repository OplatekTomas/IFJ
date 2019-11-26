//
// Created by kolba on 19.11.2019.
//

#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef enum {
    TYPE_NONE,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
} typeValue;

#define TABLE_SIZE 97

struct symTable {
    char* id;
    typeValue type;
    void* dataPtr;
    int argNum;
    int* arguments;
    int scope;
    struct symTable* ptrNext;
};

typedef struct symTable SymTable;

SymTable* hashTable[TABLE_SIZE];

unsigned int htabHashFunction(const char *str);

void deleteST(char* id);

void freeHT();

void insertST(SymTable* ptr);

SymTable* searchST(char* id);

#endif //IFJ_SYMTABLE_H
