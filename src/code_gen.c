//
// Created by tomas on 27.11.19.
//

#include "code_gen.h"

    int size = 0;
    ASTNode** result = get_postorder(tree, &size);
    printHT(table);
    free(result);
}

