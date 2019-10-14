#include "error.h"

void throw_err(error_type et){
    fprintf(stderr, "Chyba.\n"); //DODELAT!!!
    exit(et);
}