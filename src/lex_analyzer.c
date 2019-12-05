//
// Created by tomas on 14.10.19.
//

#include "lex_analyzer.h"

void scanner_init(Scanner* s, FILE* source) {
    s->source = source;
    s->first_on_line = false;
    stack_init(&s->is);
    token_stack_init(&s->ts);
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

bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_num_char(char c) {
    return (c >= '0' && c<= '9');
}

bool is_ident_char(char c) {
    return is_letter(c) || is_num_char(c) || c == '_';
}

//Načte z stdin další alfanumerické slovo (může obsahovat i '_')
int read_next_word(FILE* source, char* word, int size){
    if(word == NULL){
        throw_err(INTERN_ERR);
    }
    int counter = 0;
    char c = 0;
    //Všechny znaky které můžou obsahovat identifier.
    while(is_ident_char(c = (char)getc(source))){
        if(counter-1 == size){
            size += 128;
            word = realloc(word , size*sizeof(char));
            if(word == NULL){
                throw_err(INTERN_ERR);
            }
        }
        word[counter] = c;
        counter++;
    }
    ungetc(c, source);
    //count_spaces(source);

    return counter;
}

void handle_word(FILE* source ,Token *token){
    char* word = calloc(256, sizeof(char));
    if(word == NULL){
        throw_err(INTERN_ERR);
    }
    read_next_word(source, word, 256);
    token->keywordValue = is_keyword(word);
    if(token->keywordValue != NON_KEYWORD){
        //fprintf(stderr,"Keyword: %s\n",word);
        token->type = KEYWORD;
        free(word);
    }else{
        token->type = ID;
        token->stringValue = word;
    }
}

// Vraci:
// 0 - kdyz jen zkonzumuje mezery a nechce vratit token
// 1 - kdyz chce vratit token
// 2 - kdyz najde chybu
int handle_indent(Scanner* s, Token* t){
    int indent = count_spaces(s->source);

    //znak po mezerach
    char c = (char)getc(s->source);
    ungetc(c, s->source);
    if (c == '#') {
        // nastal jednoradkovy komentar, nedelat indent
        return 0;
    }

    // je stejny indent
    if (indent == stack_top(&s->is)) {
        return 0;
    } else if (indent > stack_top(&s->is)) {
        // je vetsi indent nez predtim
        stack_push(&s->is, indent);
        t->type = INDENT;
        return 1;
    } else {
        // je mensi hodnota nez predtim
        stack_pop(&s->is);
        if (stack_top(&s->is) < indent) {
            return 2;
        }
        t->type = DEDENT;
        s->first_on_line = false;
        return 1;
    }
}

void handle_singleline_comments(FILE * source, Scanner* s) {
    do {
        char c = (char)getc(source);
        if (c == EOF || c == '\n') {
            //ungetc(c, source);
            s->first_on_line = true;
            return;
        }
    } while (true);
}

void handle_eof(Scanner* s, Token* t) {
    if (!s->first_on_line) {
        t->type = END_OF_LINE;
        ungetc(EOF, s->source);
        s->first_on_line = true;
        return;
    }
    if (stack_top(&s->is) == 0) {
        t->type = END_OF_FILE;
    } else {
        stack_pop(&s->is);
        t->type = DEDENT;
        ungetc(EOF, s->source);
    }
}

void handle_multline_string(FILE* source, Token* t){
    int max_len = 256;
    int len = 4;
    //Drobná prasárna která kontroluje jestli jsou na začátku multiline stringu opravdu 3 " za sebou
    if(getc(source) != '"' || getc(source) != '"' || getc(source) != '"'){
        t->type = ERROR;
        return;
    }
    t->stringValue = calloc(max_len, sizeof(char));
    //Kontrola jestli poslední 3 znaky jsou " a poslední z nich není escape hodnota
    while(t->stringValue[len-1] != '"' || t->stringValue[len-2] != '"' || t->stringValue[len-3] != '"' || t->stringValue[len-4] == '\\'){
        if(len+2 == max_len){ //Realloc při nedostatečné velikosti původního pole
            max_len += 128;
            t->stringValue = realloc(t->stringValue, max_len*sizeof(char));
        }
        t->stringValue[len] = (char)getc(source);
        if(t->stringValue[len] == EOF){
            return;
        }
        len++;
    }
    //Useknutí stringu tak, aby neobsahoval poslední 3 zaky (uvozovky) - ty nejsou potřeba
    t->stringValue[len-3] = 0;
    char *temp = calloc(len, sizeof(char));
    for(int i = 0; i < len; i++){
        temp[i] = t->stringValue[i+4];
    }
    free(t->stringValue);
    t->stringValue = temp;
    t->type = STRING;
}

void handle_singleline_string(FILE* source, Token* t){
    //inicializace pomocných proměnných
    int string_len = 256;
    int real_string_len = 0;
    char c;
    t->stringValue = calloc(string_len, sizeof(char));
    if(t->stringValue == NULL){
        throw_err(INTERN_ERR);
    }
    while((c = (char)getc(source)) != '\'' || t->stringValue[real_string_len-1] == '\\'){       //načítáme znak dokuď to není ' bez escape sekvence
        if(real_string_len - 1 == string_len){      //při nedostatečné velikosti původního pole ho zvětšíme.
            string_len += 128;
            t->stringValue = realloc(t->stringValue, string_len* sizeof(char));
        }
        if(c == '\n'){      //V případě nového řádku nastala chyba z toho důvodu, že string nebyl ukončen, ale pokračuje se na další řádek
            t->type = ERROR;
            free(t->stringValue);
            return;
        }
        t->stringValue[real_string_len] = c;
        real_string_len++;
    }
}

void handle_number(FILE* source, Token* t) {
    char c;
    // 20 cisel bude snad stacit, kdyztak se prida
    int var_len = 20;
    char* variable = calloc(var_len, sizeof(char));
    int num_len = 0;
    bool dot = false;
    while(is_num_char(c = (char)getc(source)) || c == '.'){
        if (num_len + 1 == var_len) {
            var_len *= 10;
            variable = realloc(variable, var_len*sizeof(char));
        }
        if(c == '.'){ //v pripadě detekování '.' bude číslo konvertováno na float místo int
            dot = true;
        }
        variable[num_len] = c;
        num_len++;
    }
    ungetc(c, source);
    if (dot) {
        double d = strtod(variable, NULL);
        t->numberVal.d = d;
        t->type = FLOAT;
    } else {
        int i = (int)strtol(variable, NULL, 10);
        t->numberVal.i = i;
        t->type = INT;
    }
    free(variable);
}

void handle_comparison(FILE* source, Token* t, char c) {
    char next = (char)getc(source);
    switch (c) {
        case '<':
            if (next == ' ' || is_ident_char(next)) {
                ungetc(next, source);
                t->type = LESSER;
                return;
            } else if (next == '=') {
                next = (char)getc(source);
                if (next == ' ' || is_ident_char(next)) {
                    ungetc(next, source);
                    t->type = LESSER_OR_EQ;
                    return;
                }
            }
            break;
        case '>':
            if (next == ' ' || is_ident_char(next)) {
                ungetc(next, source);
                t->type = GREATER;
                return;
            } else if (next == '=') {
                next = (char)getc(source);
                if (next == ' ' || is_ident_char(next)) {
                    ungetc(next, source);
                    t->type = GREATER_OR_EQ;
                    return;
                }
            }
            break;
        case '=':
            if (next == ' ' || is_ident_char(next)) {
                ungetc(next, source);
                t->type = ASSIGN;
                return;
            } else if (next == '=') {
                next = (char)getc(source);
                if (next == ' ' || is_ident_char(next)) {
                    ungetc(next, source);
                    t->type = EQ;
                    return;
                }
            }
            break;
        case '!':
            if (next == '=') {
                next = (char)getc(source);
                if (next == ' ' || is_ident_char(next)) {
                    ungetc(next, source);
                    t->type = NON_EQ;
                    return;
                }
            }
            break;
        default:
            break;
    }
    t->type = ERROR;
}

Token get_new_token(Scanner* s) {
    Token t;
    t.type = ERROR;
    t.keywordValue = NON_KEYWORD;
    t.stringValue = NULL;
    FILE* source = s->source;
    char c;
    // cteni souboru znak po znaku
    while (true) {
        c = (char)getc(source);
        switch (c) {
            case '\n':;
                if (!s->first_on_line) {
                    t.type = END_OF_LINE;
                    s->first_on_line = true;
                    return t;
                } else {
                    ungetc(c, source);
                    s->first_on_line = false;
                    int value = handle_indent(s, &t);
                    if (value == 0) {
                        continue;
                    } else if (value == 1) {
                        return t;
                    } else {
                        t.type = ERROR;
                        return t;
                    }
                }
                // nebude fungovat poradne dokud nebudem rozpoznavat vsechno
            case ' ':
                if (s->first_on_line) {
                    ungetc(c, source);
                    s->first_on_line = false;
                    int value = handle_indent(s, &t);
                    if (value == 0) {
                        continue;
                    } else if (value == 1) {
                        return t;
                    } else {
                        t.type = ERROR;
                        return t;
                    }
                } else {
                    continue;
                }
                break;
            case ':':
                t.type = COLON;
                return t;
            case ',':
                t.type = COMMA;
                return t;
            case '(':
                t.type = OPEN_PARENTHES;
                return t;
            case ')':
                t.type = CLOSE_PARENTHES;
                return t;
            case '+':
                t.type = ADD;
                return t;
            case '-':
                t.type = SUB;
                return t;
            case '*':
                t.type = MUL;
                return t;
            case '/':
                if((c = (getc(source))) == '/'){
                    t.type = DOUBLE_DIV;
                }
                else{
                    ungetc(c, source);
                    t.type = DIV;
                }
                return t;
            case '\'':
                t.type = STRING;
                handle_singleline_string(source, &t);
                return t;
            case '=':
            case '<':
            case '>':
            case '!':
                handle_comparison(source, &t, c);
                return t;
            case '#':
                handle_singleline_comments(source, s);
                s->first_on_line = true;
                t.type = END_OF_LINE;
                return t;
            case '"':
                ungetc(c, source);
                handle_multline_string(source, &t);
                return t;
            case EOF:
                handle_eof(s, &t);
                return t;
            default:
                //Všechny znaky kterými může začínát indentifier
                if(is_letter(c) || c == '_'){
                    ungetc(c, source);
                    if (s->first_on_line) {
                        int value = handle_indent(s, &t);
                        if (value == 1) {
                            s->first_on_line = true;
                            return t;
                        } else if (value != 0) {
                            t.type = ERROR;
                            return t;
                        }
                    }
                    handle_word(source, &t);
                    s->first_on_line = false;
                    return t;
                } else if (is_num_char(c)) {
                    ungetc(c, source);
                    handle_number(source, &t);
                    return t;
                }
                break;
        }
    }
}

Token get_next_token(Scanner* s){
    if (token_stack_empty(&s->ts)) {
        return get_new_token(s);
    } else {
        Token t = token_stack_top(&s->ts);
        token_stack_pop(&s->ts);
        return t;
    }
}

void scanner_unget(Scanner* s, Token t) {
    token_stack_push(&s->ts, t);
}

