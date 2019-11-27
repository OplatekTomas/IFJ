//
// Created by kolba on 19.11.2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define SIZE 97

#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

typedef enum {
    TYPE_NONE,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
} typeValue;

typedef struct arguments{
    char* id;
    typeValue type;
};

typedef struct symTable{
    char* id;
    typeValue type;
    void* dataPtr;
    int argNum;
    struct arguments* args; //TODO asi nefunguje
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
