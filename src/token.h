#ifndef TOKEN_H
#define TOKEN_H

#include "string.h"
#include "source_code_position.h"

typedef struct token_s {
    int type;
    string_t* value;
    source_code_position_t* position;
} token_t;

token_t* token_create(int type, string_t* value, source_code_position_t* source_code_position);
void token_deallocate(token_t* token); 

#endif
