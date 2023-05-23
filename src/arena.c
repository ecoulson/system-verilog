#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "arena.h"

arena_t* arena_create(size_t block_size) {
    arena_t* arena = malloc(sizeof(arena_t));

    arena->block_size = block_size;
    arena->block = malloc(block_size);
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
    
    if (arena->block_size < offset + size) {
        printf("out of mem\n");
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

void* arena_zero_new_memory(arena_t* arena, void* old_memory, size_t old_size, size_t new_size) {
    if (old_size < new_size) {
        memset(&arena->block[arena->current_offset], 0, new_size - old_size);
    }
    return old_memory;
}

void* arena_reallocate(arena_t* arena, void* old_memory, size_t old_size, size_t new_size) {
    if (old_memory == NULL || old_size == 0) {
        return arena_allocate(arena, new_size);
    }

    if (old_memory < arena->block || arena->block + arena->block_size <= old_memory) {
        assert(0 && "Memory is outside the bounds of the arena");
        return NULL;
    }

    if (arena->block + arena->previous_offset == old_memory) {
        arena->current_offset = arena->previous_offset + new_size;
        return arena_zero_new_memory(arena, old_memory, old_size, new_size);
    }

    void* new_memory = arena_allocate(arena, new_size);
    size_t reallocation_size = old_size < new_size ? old_size : new_size;
    memmove(new_memory, old_memory, reallocation_size);

    return new_memory;
}
