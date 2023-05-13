#ifndef STRING_H
#define STRING_H
#include <stdio.h>

#include "array_list.h"

typedef struct string_s {
    char* value;
    int length;
} string_t;

string_t* string_create(char* value); 
void string_deallocate(string_t* string); 
string_t* string_append_character(string_t* string, char character);
string_t* string_format(string_t* format_string, array_list_t* arguments);
string_t* string_concatenate(string_t* prefix, string_t* suffix);

#endif
