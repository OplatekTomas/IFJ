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
 * symtable.h
 */
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
    bool has_been_defined;
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

bool is_symbol_global(SymTable* search, SymTable** table);

void fill_with_fn(SymTable** hashTable);

#endif //IFJ_SYMTABLE_H
