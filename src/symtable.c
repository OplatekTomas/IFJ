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
 * symtable.c
 */

#include "symtable.h"

/*
 * Funkce allocHT nemá žádné parametry a slouží k alokaci hash tabulky,
 * pomocí které je implementována tabulka symbolů. Inicializuje všechny ukazatele na sloupce na NULL.
 * Funkce vrací ukazatel na první ukazatel na prvek SymTable.
 */
SymTable** allocHT(){
    SymTable** ptr = (SymTable**)malloc(TABLE_SIZE* sizeof(SymTable*));
    addPtr(ptr);
    for(int i = 0; i < TABLE_SIZE; i++){
        ptr[i] = NULL;
    }
    //addPtr(ptr);
    return ptr;
}

/*
 * Funkce allocArgs nemá žádné parametry a slouží k alokaci a inicializaci
 * pole struktur Arguments v SymTable.
 * Vrací ukazatel na první prvek pole.
 */
Arguments* allocArgs(){
    Arguments* ptr = (Arguments*)malloc(sizeof(Arguments));
    ptr->nextArg = NULL;
    ptr->id = NULL;
    ptr->type = TYPE_NONE;
    addPtr(ptr);
    return ptr;
}

/*
 * Funkce allocST má parametr ukazatel na pole znaků id.
 * Slouží k alokaci a inicializaci struktury SymTable.
 * Vrací ukazatel na strukturu SymTable.
 */
SymTable* allocST(char* id){
    SymTable* ptr = (SymTable*)malloc(sizeof(SymTable));
    if(ptr == NULL)
        return NULL;
    ptr->has_been_defined = false;
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

/*
 * Funkce htabHashFunction má parametr konstantní ukazatel na pole znaků str.
 * Slouží ke generaci hashe pro hash tabulku.
 * Funkce vrací hash jako bezznaménkový integer.
 */
unsigned int htabHashFunction(const char *str) {
    uint32_t h=0;     // musí mít 32 bitů
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h % TABLE_SIZE;
}

/*
 * Funkce deleteST má parametry ukazatel na ukazatel na první sloupec hash tabulky hashTable
 * a ukazatel na pole znaků id.
 * Slouží k vyhledání struktury SymTable v hash tabulce podle daného id
 * a odstranění dané struktury.
 */
void deleteST(SymTable** hashTable, char* id){
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

/*
 * Funkce insertST má parametry ukazatel na ukazatel na první sloupec hashTable
 * a ukazatel na strukturu SymTable ptr
 * Slouží k vloží již alokované a naplněné struktury SymTable do hashTable.
 */
void insertST(SymTable** hashTable, SymTable* ptr){
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

/*
 * Funkce searchST má parametry ukazatel na ukazatel na první sloupec hashTable,
 * ukazatel na pole znaků id a ukazatel na pole znaků funcID.
 * Slouží k vyhledání struktury SymTable v hashTable podle id.
 * Vrací ukazatel na nalezenou strukturu SymTable, pokud ji nenajde vrací NULL.
 */
SymTable* searchST(SymTable** hashTable, char* id, char* funcID){
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
    if (searchST(table, search->id, NULL) == NULL) {
        return false;
    }
    return true;
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
