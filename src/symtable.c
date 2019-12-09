//
// Created by kolba on 19.11.2019.
//

#include "symtable.h"

SymTable** allocHT(){
    SymTable** ptr = (SymTable**)malloc(TABLE_SIZE* sizeof(SymTable*));
    addPtr(ptr);
    for(int i = 0; i < TABLE_SIZE; i++){
        ptr[i] = NULL;
    }
    //addPtr(ptr);
    return ptr;
}

Arguments* allocArgs(){
    Arguments* ptr = (Arguments*)malloc(sizeof(Arguments));
    ptr->nextArg = NULL;
    ptr->id = NULL;
    ptr->type = TYPE_NONE;
    addPtr(ptr);
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
    ptr->localTable = NULL;
    addPtr(ptr);
    return ptr;
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
    } else {
        prev->ptrNext = item->ptrNext;
    }
    ////free(item);
}

void insertST(SymTable** hashTable, SymTable* ptr){    //vloží již alokovanou a vypněnou symTable do hashTable
    if(ptr != NULL){
        SymTable* temp = searchST(hashTable, ptr->id,NULL);
        if(temp != NULL){ //Fucking band-aid
            return;
        }
        int hash = htabHashFunction(ptr->id);
        if(&hashTable[hash] != NULL){
            ptr->ptrNext = hashTable[hash];
        }
        hashTable[hash] = ptr;
    }
}

SymTable* searchST(SymTable** hashTable, char* id, char* funcID){    //vyhledá symTable v hashTable a vrátí pointer na ní
    if(id == NULL){
        return NULL;
    }
    char* searchedID = id;
    //printHT(hashTable);
    if(funcID != NULL)
        searchedID = funcID;
    int hash = htabHashFunction(searchedID);
    SymTable* ptr = hashTable[hash];
    if(ptr == NULL)
        return NULL;
    while(strcmp(ptr->id, searchedID)){
        if(ptr->ptrNext == NULL)
            return NULL;
        ptr = ptr->ptrNext;
    }
    if(funcID != NULL) {
        ptr = searchST(ptr->localTable, id, NULL);
        if(ptr == NULL){
            ptr = searchST(hashTable, id, NULL);
        }
    }
    return ptr;
}

void printHT(SymTable** ht){
    for(int i = 0; i < TABLE_SIZE; i++){
        SymTable *item = ht[i];
        if(item == NULL){
            continue;
        }
        fprintf(stderr,"%d.  ", i+1);
        SymTable* prev = NULL;
        while(item != NULL){
           fprintf(stderr,"%s ->", item->id);
           if(prev == item){
               break;
           }
           prev = item;
           item = item->ptrNext;
        }
        fprintf(stderr,"\n");
    }
}

bool is_symbol_global(SymTable* search, SymTable** table) {
    
}


void freeHT(SymTable** hashTable){  //vymaže celou hashTable
    /*if(hashTable == NULL)
        return;
    //printHT(hashTable);
    for(int i = 0; i < TABLE_SIZE; i++){
        SymTable *item = hashTable[i];
        while(item != NULL){
            SymTable *tmpST = item->ptrNext;
            //free(item->id);
            Arguments* args = item->args;
            while(args != NULL){
                Arguments* argsNext = args->nextArg;
                //free(args);
                args = argsNext;
            }
            if(item->localTable != NULL){

                freeHT(item->localTable);
                item->localTable = NULL;
            }

            //free(item);
            item = tmpST;
        }
        hashTable[i] = NULL;
    }
    //free(hashTable);
    hashTable = NULL;*/
}

void fill_with_fn(SymTable **hashTable) {
    char* functions[] = {"inputs", "inputi", "inputf", "print", "len", "substr", "ord", "chr"};
    int argCount[] = {0, 0, 0, -1, 1, 3, 2, 1};
    for(int i = 0; i < 8; i++){
        char* ptr = malloc(sizeof(char)* 16);
        addPtr(ptr);
        strcpy(ptr, functions[i]);
        SymTable* item = allocST(ptr);
        item->type = TYPE_FUNCTION;
        item->argNum = argCount[i];
        insertST(hashTable, item);
    }
}
