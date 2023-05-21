#ifndef ARENA_H
#define ARENA_H

#define DEFAULT_ALIGNMENT (2 * sizeof(void*))

#include <stdlib.h>

typedef struct arena_s {
    void* block;
    size_t block_size;
    size_t previous_offset;
    size_t current_offset;
} arena_t;

arena_t* arena_create(size_t block_size);
void* arena_allocate(arena_t* arena, size_t size);
void arena_free(arena_t* arena);

#endif
