#include <stdlib.h>
#include <stdio.h>

#include "array_list.h"

array_list_t* array_list_create() {
    array_list_t* array_list = malloc(sizeof(array_list_t));

    if (array_list == NULL) {
        printf("Failed to allocate memory for creating the array list\n");
        exit(1);
    }

    array_list->capacity = ARRAY_LIST_INITIAL_CAPACITY;
    array_list->length = 0;
    array_list->elements = malloc(ARRAY_LIST_INITIAL_CAPACITY * sizeof(void*));

    if (array_list->elements == NULL) {
        printf("Failed to allocate memory for storing the elements of the array list\n");
        exit(1);
    }

    return array_list;
}

void array_list_deallocate(array_list_t* list) {
    free(list->elements);
    free(list);
}

void array_list_upsize(array_list_t* array_list) {
    array_list->capacity <<= 1;
    array_list->elements = realloc(array_list->elements, array_list->capacity * sizeof(void*));

    if (array_list->elements == NULL) {
        printf("Failed to allocate memory for resizing array list\n");
        exit(1);
    }
}

void* array_list_add(array_list_t *array_list, int i, void *element) {
    if (array_list->capacity == array_list->length) {
        array_list_upsize(array_list);
    }

    array_list->length++;

    for (size_t j = array_list->length - 1; j > i; j--) {
        array_list->elements[j] = array_list->elements[j - 1];
    }
    
    array_list->elements[i] = element;

    return element;
}

void array_list_assert_in_bounds(array_list_t *array_list, int i) {
    if (i < 0 || i >= array_list->length) {
        printf(
            "Attempting to access index %d, which is outside the bounds of the list of length %d\n",
            i,
            array_list->length
        );
        exit(1);
    }
}

void* array_list_get(array_list_t *array_list, int i) {
    array_list_assert_in_bounds(array_list, i);
    return array_list->elements[i];
}

void array_list_downsize(array_list_t* array_list) {
    if (array_list->length > 1) {
        array_list->capacity >>= 1;
        array_list->elements = realloc(array_list->elements, array_list->capacity * sizeof(void*));

        if (array_list->elements == NULL) {
            printf("Failed to allocate memory for resizing array list\n");
            exit(1);
        }
    }
}

void* array_list_remove(array_list_t *array_list, int i) {
    array_list_assert_in_bounds(array_list, i);
    void *element = array_list->elements[i];

    for (size_t j = i; j < array_list->length - 1; j++) {
        array_list->elements[j] = array_list->elements[j + 1];
    }

    array_list->length--;

    if (array_list->capacity >= array_list->length * 2) {
        array_list_downsize(array_list);
    }

    return element;
}

void* array_list_set(array_list_t *array_list, int i, void *element) {
    array_list_assert_in_bounds(array_list, i);
    array_list->elements[i] = element;
    return element;
}
