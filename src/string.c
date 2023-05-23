#include <string.h>
#include <math.h>
#include <stdio.h>

#include "string.h"
#include "arena.h"

string_t* string_create(arena_t* arena, char* value) {
    string_t* string = arena_allocate(arena, sizeof(string_t));

    string->length = strlen(value);
    string->value = arena_allocate(arena, sizeof(char) * (string->length + 1));
    
    for (size_t i = 0; i < string->length; i++) {
        string->value[i] = value[i];
    }
    string->value[string->length] = '\0';
    
    return string;
}

string_t* string_append_character(arena_t* arena, string_t *string, char character) {
    int old_size = (string->length + 1) * sizeof(char);
    string->length++;
    int new_size = (string->length + 1) * sizeof(char);

    string->value = arena_reallocate(arena, string->value, old_size, new_size);
    string->value[string->length - 1] = character;
    string->value[string->length] = '\0';

    return string;
}

string_t* string_concatenate(arena_t* arena, string_t *prefix, string_t *suffix) {
    int old_size = (prefix->length + 1) * sizeof(char);
    prefix->length += suffix->length;
    int new_size = (prefix->length + 1) * sizeof(char);
    prefix->value = arena_reallocate(arena, prefix->value, old_size, new_size);
    prefix->value = strcat(prefix->value, suffix->value);

    return prefix;
}

string_t* string_format(arena_t* arena, string_t* format_string, array_list_t* arguments) {
    string_t *display_string = string_create(arena, "");
    int parameter_index = 0;

    for (int i = 0; i < format_string->length; i++) {
        char ch = format_string->value[i];

        if (ch == '\{') {
            display_string = string_append_character(arena, display_string, '{');
            continue;
        }

        if (ch != '{') {
            display_string = string_append_character(arena, display_string, ch);
            continue;
        }

        if (i + 1 == format_string->length) {
            printf("Illegal format string, all '{' must have a closing '}'.\n");
            exit(1);
            return NULL;
        }

        char closing_bracket = format_string->value[i + 1];

        if (closing_bracket != '}') {
            printf("Expected a closing '}' bracket.\n");
            exit(1);
            return NULL;
        }

        display_string = string_concatenate(arena, display_string, array_list_get(arguments, parameter_index));
        parameter_index++;
        i++;
    }

    display_string->value[display_string->length] = '\0';

    return display_string;
}
