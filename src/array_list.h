#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
#define ARRAY_LIST_INITIAL_CAPACITY 8

#include <stdlib.h>

#include "arena.h"

typedef struct array_list_s
{
    arena_t* arena;
    int length;
    int capacity;
    void **elements;
} array_list_t;

array_list_t* array_list_create(arena_t* arena); 
void* array_list_add(array_list_t *array_list, int i, void *element);
void* array_list_get(array_list_t *array_list, int i);
void* array_list_remove(array_list_t *array_list, int i);
void* array_list_set(array_list_t *array_list, int i, void *element);

#endif
