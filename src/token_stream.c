#include "token_stream.h"

token_stream_t* token_stream_create(arena_t* arena) {
    token_stream_t* token_stream = arena_allocate(arena, sizeof(token_stream_t));

    token_stream->position = 0;
    token_stream->stream = array_list_create(arena);

    return token_stream;
}

token_t* token_stream_read(token_stream_t* token_stream) {
    return array_list_get(token_stream->stream, token_stream->position++);
}

token_t* token_stream_peek(token_stream_t* token_stream) {
    return array_list_get(token_stream->stream, token_stream->position);
}

void token_stream_write(token_stream_t* token_stream, token_t* token) {
    array_list_add(token_stream->stream, token_stream->stream->length, token);
}

int token_stream_can_read(token_stream_t* token_stream) {
    return token_stream->position < token_stream->stream->length;
}
