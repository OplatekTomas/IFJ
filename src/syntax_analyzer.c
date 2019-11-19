#include "syntax_analyzer.h"

char token_type[][100] = {
        "ID",
        "ASSIGN",
        "GREATER",
        "LESSER",
        "GREATER_OR_EQ",
        "LESSER_OR_EQ",
        "EQ",
        "NON_EQ",
        "OPEN_PARENTHES",
        "CLOSE_PARENTHES",
        "INT",
        "BOOL",
        "FLOAT",
        "STRING",
        "KEYWORD",
        "INDENT",
        "DEDENT",
        "END_OF_FILE",
        "END_OF_LINE",
        "NIL",
        "ERROR",
        "COLON",
        "COMMA",
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "DOUBLE_DIV"
};

<<<<<<< Updated upstream
void get_derivation_tree(FILE *source){
    Parser p;
    parser_init(&p, source);
    Token t;
    while(true){
        t = get_next_token(&p);
        if (t.type == INT) {
            printf("INT %i\n", t.numberVal.i);
        } else if (t.type == FLOAT) {
            printf("FLOAT %f\n", t.numberVal.d);
        } else if (t.type == STRING) {
            printf("STRING '%s'\n", t.stringValue);
            free(t.stringValue);
        } else if (t.type == KEYWORD) {
            printf("%s\n", KEYWORDS[t.keywordValue]);
        } else if (t.type == ID) {
            printf("ID %s\n", t.stringValue);
            free(t.stringValue);
        } else if (t.type!= ERROR) {
            printf("%s\n", token_type[t.type]);
        }
        // docasne
        if (t.type == END_OF_FILE) {
            break;
=======
bool check_function_call(ASTNode* tree, Scanner* s) {
    //TODO: dodelat
    return false;
}

bool check_expression(ASTNode* tree, Scanner* s) {
     printf("kontrola vyrazu\n");
    // TODO: dodelat
}

bool check_assignment(ASTNode* tree, Scanner* s, char* left_side) {
    //TODO: dodelat
    printf("kontrola prirazeni\n");
    if (check_expression(tree, s)) {
        return true;
    } else {
        return false;
    }
}

bool check_if(ASTNode* tree, Scanner* s) {
    //TODO: dodelat
    return false;
}

int check_while(ASTNode* tree, Scanner* s) {
    int next_token;
    if(!check_cond(tree, s))
        return false;

    Token token = get_next_token(s);
    if(token.type != COLON)
        return false;

    token = get_next_token(s);
    if(token.type != INDENT)
        return false;

    do{
        next_token = check_block(tree, s);
        if(!next_token)
            return next_token;
    } while(next_token != 4);           //dokud nenastane DEDENT

    return true;
}

bool check_definition(ASTNode* tree, Scanner* s) {
    //TODO: dodelat
    return false;
}

/// Vraci   0 - kdyz nastala lexikalni chyba
///         1 - kdyz nastala syntakticka chyba
///         2 - kdyz nenastala chyba
///         3 - kdyz nastal konec souboru
bool check_block(ASTNode* tree, Scanner* s) {
    Token t = get_next_token(s);

    printf("kontrola bloku\n");

    switch (t.type) {
        case ID:;
            char* id = t.stringValue;
            t = get_next_token(s);
            switch (t.type) {
                case ASSIGN:
                    return check_assignment(tree, s, id);
                case OPEN_PARENTHES:
                    return check_function_call(tree, s);
                default:
                    return 1;
            }
        case KEYWORD:
            switch (t.keywordValue) {
                case IF:
                    return check_if(tree, s);
                case WHILE:
                    return check_while(tree, s);
                case DEF:
                    return check_definition(tree, s);
                default:
                    return 1;
            }
        case STRING:
            //TODO: Poresit multiline stringy
            return 1;
        case END_OF_FILE:
            return 3;
        default:
            return 1;
    }
}

/// Vraci derivacni strom, ktery je potom potreba uvolnit
ASTNode* get_derivation_tree(FILE *source) {
    Scanner s;
    scanner_init(&s, source);

    ASTNode* root = (ASTNode*)malloc(sizeof(ASTNode));

    while (true) {
        int result = check_block(root ,&s);
        switch (result) {
            case 0:
                fprintf(stderr, "nastala lexikalni chyba\n");
                free_tree(root);
                return NULL;
            case 1:
                fprintf(stderr, "nastala syntakticka chyba\n");
                free_tree(root);
                return NULL;
            case 3:
                break;
            default:
                continue;
>>>>>>> Stashed changes
        }
    }
}
