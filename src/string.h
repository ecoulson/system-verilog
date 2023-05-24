#ifndef STRING_H
#define STRING_H
#include <stdio.h>

#include "array_list.h"
#include "arena.h"

typedef struct string_s {
    char* value;
    int length;
} string_t;

string_t* string_create(arena_t* arena, char* value); 
string_t* string_append_character(arena_t* arena, string_t* string, char character);
string_t* string_format(arena_t* arena, string_t* format_string, array_list_t* arguments);
string_t* string_concatenate(arena_t* arena, string_t* prefix, string_t* suffix);
string_t* string_convert_integer(int i);
int string_equals_constant(string_t* string, char* constant);

#endif
