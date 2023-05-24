#include "token.h"

token_t* token_create(arena_t* arena, token_type_t type, string_t* value, source_code_position_t* source_code_position) {
    token_t* token = arena_allocate(arena, sizeof(token_t));

    token->type = type;
    token->value = value;
    token->position = source_code_position;

    return token;
}
