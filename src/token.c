#include "token.h"

token_t* token_create(int type, string_t* value, source_code_position_t* source_code_position) {
    token_t* token = malloc(sizeof(token_t));

    token->type = type;
    token->value = value;
    token->position = source_code_position;

    return token;
}

void token_deallocate(token_t* token) {
    free(token);
}
