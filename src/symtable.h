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

#define TABLE_SIZE 97

typedef enum {
    TYPE_NONE,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
} typeValue;

struct arguments{
    char* id;
    typeValue type;
    struct arguments* nextArg;
};

typedef struct arguments Arguments;

struct symTable {
    char* id;
    typeValue type;
    void* dataPtr;
    int argNum;
    struct arguments* args;
    int scope;
    struct symTable* ptrNext;
};

typedef struct symTable SymTable;

SymTable* hashTable;

SymTable* allocHT();

unsigned int htabHashFunction(const char *str);

Arguments* allocArgs();

void deleteST(char* id);

void freeHT();

void insertST(SymTable* ptr);

SymTable* searchST(char* id);

#endif //IFJ_SYMTABLE_H
