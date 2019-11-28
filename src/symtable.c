//
// Created by kolba on 19.11.2019.
//

#include "symtable.h"

SymTable** allocHT(){
    SymTable** ptr = (SymTable**)malloc(TABLE_SIZE* sizeof(SymTable*));
    for(int i = 0; i < TABLE_SIZE; i++){
        ptr[i] = NULL;
    }
    return ptr;
}

Arguments* allocArgs(){
    Arguments* ptr = (Arguments*)malloc(sizeof(Arguments));
    ptr->nextArg = NULL;
    ptr->id = NULL;
    ptr->type = TYPE_NONE;
    return ptr;
}

SymTable* allocST(char* id){
    SymTable* ptr = (SymTable*)malloc(sizeof(SymTable));
    if(ptr == NULL)
        return NULL;
    ptr->ptrNext = NULL;
    ptr->id = id;
    ptr->argNum = 0;
    ptr->args = NULL;
    ptr->dataPtr = NULL;
    ptr->scope = 0;
    ptr->type = TYPE_NONE;
}

unsigned int htabHashFunction(const char *str) {    //funkce pro generování hashe podle id
    uint32_t h=0;     // musí mít 32 bitů
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h % TABLE_SIZE;
}

void deleteST(SymTable** hashTable, char* id){    //odstraní symTable z hashTable
    if(id == NULL)
        return;
    int hash = htabHashFunction(id);
    SymTable* item = hashTable[hash];
    SymTable* prev = hashTable[hash];
    if(item == NULL){
        return;
    }
    while(strcmp(item->id, id) != 0){
        if(item->ptrNext == NULL){
            return;
        }
        prev = item;
        item = item->ptrNext;
    }
    if(item == hashTable[hash]){
        hashTable[hash] = item->ptrNext;
    }else{
        prev->ptrNext = item->ptrNext;
    }
    free(item);
}

void insertST(SymTable** hashTable, SymTable* ptr){    //vloží již alokovanou a vypněnou symTable do hashTable
    if(ptr != NULL){
        int hash = htabHashFunction(ptr->id);
        if(&hashTable[hash] != NULL){
            ptr->ptrNext = hashTable[hash];
        }
        hashTable[hash] = ptr;
    }
}

SymTable* searchST(SymTable** hashTable, char* id){    //vyhledá symTable v hashTable a vrátí pointer na ní
    if(id == NULL){
        return NULL;
    }
    int hash = htabHashFunction(id);
    struct symTable* ptr = hashTable[hash];
    if(ptr == NULL)
        return NULL;
    while(strcmp(ptr->id, id)){
        if(ptr->ptrNext == NULL)
            return NULL;
        ptr = ptr->ptrNext;
    }
    return ptr;
}

void printHT(SymTable** ht){
    for(int i = 0; i < TABLE_SIZE; i++){
        SymTable *item = ht[i];
        if(item == NULL){
            continue;
        }
        printf("%d.  ", i+1);
        while(item != NULL){
           printf("%s", item->id);
           item = item->ptrNext;
        }
        printf("\n");
    }
}


void freeHT(SymTable** hashTable){  //vymaže celou hashTable
    if(hashTable == NULL)
        return;
    for(int i = 0; i < TABLE_SIZE; i++){
        SymTable *item = hashTable[i];
        while(item != NULL){
            SymTable *tmpST = item->ptrNext;
            free(item->id);
            Arguments* args = item->args;
            while(args != NULL){
                Arguments* argsNext = item->args->nextArg;
                free(item->args);
                args = argsNext;
            }
            free(item);
            item = tmpST;
        }
        hashTable[i] = NULL;
    }
    free(hashTable);
}
