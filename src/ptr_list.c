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
 * ptr_list.c
 */

#include "ptr_list.h"

int ptrArrSize = 0;
int maxPtrArrSize = 1024;

void initPtrList(){
    arr = malloc(sizeof(void*) * maxPtrArrSize);
    if(arr == NULL){
        throw_err(INTERN_ERR);
    }
}

void changePtr(void *old, void *new){
    for(int i = 0; i < ptrArrSize; i++){
        if(arr[i] == old){
            arr[i] = new;
            break;
        }
    }
}

void addPtr(void *ptr){
    if(ptrArrSize >= maxPtrArrSize){
        maxPtrArrSize += 1024;
        arr = realloc(arr,sizeof(void*) * maxPtrArrSize);
    }
    arr[ptrArrSize] = ptr;
    ptrArrSize++;

}

int getSize(){
    return ptrArrSize;
}

void freeAllPtr(){
    for(int i = 0; i < ptrArrSize; i++){
        free(arr[i]);
    }
    free(arr);
}
