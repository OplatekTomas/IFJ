//
// Created by tomas on 14.10.19.
//

#include "lex_analyzer.h"


//Načte z stdin další alfanumerické slovo (může obsahovat i '_')
int read_next_word(FILE* source, char* word, int size){
    if(word == NULL){
        //throw
    }
    int counter = 0;
    char c = 0;
    //Všechny znaky které můžou obsahovat identifier.
    while(((c = getc(source)) >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c<= '9') || c == '_'){
        if(counter-1 == size){
            size += 128;
            word = realloc(word , size*sizeof(char));
        }
        word[counter] = c;
        counter++;
    }
    return counter;
}

void handle_word(FILE* source ,Token *token){
    char* word = calloc(256, sizeof(char));
    if(word == NULL){
        //throw
    }
    int len = read_next_word(source, word, 256);
    token->keywordValue = is_keyword(word);
    if(token->keywordValue != NON_KEYWORD){
        printf("Keyword: %s\n",word);
        token->type = KEYWORD;
        free(word);
        return;
    }
    free(word);
}

int count_spaces(FILE* source) {
    int spaces = 0;
    char c;
    do {
        c = (char)getc(source);
        if (c == ' ') {
            spaces++;
        } else {
            ungetc(c, source);
            break;
        }
    } while (true);
    return spaces;
}

void handle_indent(FILE* source, IndentStack *is, Token *token){
    int indent = count_spaces(source);
    // je jiny indent nez na topu
    if (indent != stack_top(is)) {
        // pridani indentu
        if (indent > stack_top(is)) {
            stack_push(is, indent);
            token->type = INDENT;
            printf("INDENT\n");
        } else if (indent < stack_top(is)) {
            // ubrani indentu
            do {
                stack_pop(is);
                token->type = DEDENT;
                printf("DEDENT\n");
                // jsme na spravne urovni dedentu?
                if (stack_top(is) == indent) {
                    break;
                } else if (stack_top(is) < indent) {
                    // indent muze byt mensi nez top jedine v pripade ze se popovalo a nenasla se hodnota topu, takze nastala indent chyba
                    exit(1);
                }
            } while (stack_top(is) != 0);
        }
    }
}


Token get_next_token(FILE* source){
    Token t;
    char c;
    IndentStack is;
    stack_init(&is);
    // cteni souboru znak po znaku
    do {
        c = (char)getc(source);
        switch (c) {
            case '\n':
                handle_indent(source, &is, &t);
                break;
            case '"':
                break;
            default:
                //Všechny znaky kterými může začínát indentifier
                if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'){
                    ungetc(c, source);
                    handle_word(source, &t);
                }

                break;
        }
    } while (c != EOF);

    // pridani dedentu na konci pro vyrovani s indentama
    while (stack_top(&is) != 0) {
        stack_pop(&is);
        printf("DEDENT\n");
    }
    return t;
}
