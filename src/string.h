#ifndef STRING_H
#define STRING_H
#include <stdio.h>

typedef struct string_s {
    char* value;
    int length;
} string_t;

string_t* create_string(char* value); 
void free_string(string_t* string); 
void print_string(FILE* stream, string_t* string);

#endif
