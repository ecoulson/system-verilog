#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

#include "array_list.h"
#include "arena.h"
#include "token.h"

typedef struct token_stream_s {
    array_list_t* stream;
    int position;
} token_stream_t;

token_stream_t* token_stream_create(arena_t* arena);
token_t* token_stream_read(token_stream_t* token_stream);
token_t* token_stream_peek(token_stream_t* token_stream);
void token_stream_write(token_stream_t* token_stream, token_t* token);
int token_stream_can_read(token_stream_t* token_stream);

#endif
