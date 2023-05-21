#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "arena.h"

arena_t* arena_create(size_t block_size) {
    arena_t* arena = malloc(sizeof(arena_t));

    arena->block_size = block_size;
    arena->block = malloc(sizeof(block_size));
    arena->current_offset = 0;
    arena->previous_offset = 0;

    return arena;
}

int arena_is_power_of_two(uintptr_t pointer) {
    return (pointer & (pointer - 1)) == 0;
}

uintptr_t arena_align_allocation(uintptr_t pointer, size_t alignment) {
    uintptr_t aligned_pointer = pointer;
    assert(arena_is_power_of_two(alignment));

    uintptr_t modulo = aligned_pointer & (alignment - 1);

    if (modulo != 0) {
        aligned_pointer += alignment - modulo;
    }

    return aligned_pointer;
}

void* arena_allocate(arena_t* arena, size_t size) {
    uintptr_t current_pointer = (uintptr_t)arena->block + (uintptr_t)arena->current_offset;
    uintptr_t offset = arena_align_allocation(current_pointer, DEFAULT_ALIGNMENT);
    offset -= (uintptr_t)arena->block;
    
    if (offset + size > arena->block_size) {
        return NULL;
    }

    void* pointer = &arena->block[offset];
    arena->current_offset = offset + size;
    arena->previous_offset = offset;
    
    memset(pointer, 0, size);

    return pointer;
}

void arena_free(arena_t* arena) {
    free(arena->block);
    free(arena);
}
