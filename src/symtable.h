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
#include "ptr_list.h"

#define TABLE_SIZE 97

typedef enum {
    TYPE_NONE,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_FUNCTION
} TypeValue;

struct arguments{
    char* id;
    TypeValue type;
    struct arguments* nextArg;
};

typedef struct arguments Arguments;

struct symTable {
    char* id;
    TypeValue type;
    void* dataPtr;
    int argNum;
    struct arguments* args;
    int scope;
    struct symTable* ptrNext;
    struct symTable** localTable; //Yes. We do hate ourselves.
};

typedef struct symTable SymTable;

SymTable** allocHT();

unsigned int htabHashFunction(const char *str);

Arguments* allocArgs();

SymTable* allocST(char* id);

void printHT(SymTable** hashTable);

void deleteST(SymTable** hashTable, char* id);

void freeHT(SymTable** hashTable);

void insertST(SymTable** hashTable, SymTable* ptr);

SymTable* searchST(SymTable** hashTable, char* id, char* funcID);

void fill_with_fn(SymTable** hashTable);

#endif //IFJ_SYMTABLE_H
