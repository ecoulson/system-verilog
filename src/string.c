#include <string.h>
#include <math.h>
#include <stdio.h>

#include "string.h"

string_t* string_create(char* value) {
    string_t* string = malloc(sizeof(string_t));

    string->value = strdup(value);
    string->length = strlen(string->value);
    
    return string;
}

void string_deallocate(string_t *string) {
    free(string->value);
    free(string);
}

string_t* string_append_character(string_t *string, char character) {
    string->length++;
    string->value = realloc(string->value, sizeof(char) * (string->length + 1));
    string->value[string->length - 1] = character;
    string->value[string->length] = '\0';

    return string;
}

string_t* string_concatenate(string_t *prefix, string_t *suffix) {
    prefix->length += suffix->length;
    prefix->value = realloc(prefix->value, sizeof(char) * (prefix->length + 1));
    prefix->value = strcat(prefix->value, suffix->value);

    return prefix;
}

string_t* string_format(string_t* format_string, array_list_t* arguments) {
    string_t *display_string = string_create("");
    int parameter_index = 0;

    for (int i = 0; i < format_string->length; i++) {
        char ch = format_string->value[i];

        if (ch == '\{') {
            display_string = string_append_character(display_string, '{');
            continue;
        }

        if (ch != '{') {
            display_string = string_append_character(display_string, ch);
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

        display_string = string_concatenate(display_string, array_list_get(arguments, parameter_index));
        parameter_index++;
        i++;
    }

    display_string->value[display_string->length] = '\0';

    return display_string;
}

string_t* string_convert_integer(int i) {
    char* buffer = malloc(50 * sizeof(char));
    sprintf(buffer, "%d", i);
    string_t* result = string_create(buffer);
    free(buffer);
    return result;
}
