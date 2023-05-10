#include <stdlib.h>
#include <string.h>

#include "string.h"

string_t* create_string(char* value) {
    string_t* string = malloc(sizeof(string_t));

    string->value = strdup(value);
    string->length = strlen(string->value);
    
    return string;
}

void free_string(string_t *string) {
    free(string->value);
    free(string);
}

void print_string(FILE* file, string_t *string) {
    fprintf(file, "%s\n", string->value);
}
