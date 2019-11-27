//
// Created by kolba on 19.11.2019.
//

#include "symtable.h"

SymTable* allocHT(){
    SymTable* ptr = (SymTable*)malloc(TABLE_SIZE* sizeof(SymTable));
    return ptr;
}

Arguments* allocArgs(){
    Arguments* ptr = (Arguments*)malloc(sizeof(Arguments));
    return ptr;
}

unsigned int htabHashFunction(const char *str) {    //funkce pro generování hashe podle id
    uint32_t h=0;     // musí mít 32 bitů
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h % TABLE_SIZE;
}

void deleteST(char* id){    //odstraní symTable z hashTable
    if(id == NULL)
        return;
    int hash = htabHashFunction(id);
    struct symTable* item = hashTable[hash];
    struct symTable* prev = hashTable[hash];
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

void insertST(struct symTable* ptr){    //vloží již alokovanou a vypněnou symTable do hashTable
    if(ptr != NULL){
        int hash = htabHashFunction(ptr->id);
        if(hashTable[hash] != NULL){
            ptr->ptrNext = hashTable[hash];
        }
        hashTable[hash] = ptr;
    }
}

SymTable* searchST(char* id){    //vyhledá symTable v hashTable a vrátí pointer na ní
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

void freeHT(){  //vymaže celou hashTable
    for(int i = 0; i < TABLE_SIZE; i++){
        SymTable *item = hashTable[i];
        while(item != NULL){
            struct symTable *tmp = item->ptrNext;
            free(item);
            item = tmp;
        }
        hashTable[i] = NULL;
    }
}