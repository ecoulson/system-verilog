#include <stdlib.h>
#include <stdio.h>

#include "string.h"
#include "array_list.h"
#include "verilog_file_stream_reader.h"

int main(int argument_count, char** argument_variables) {
    string_t *string = string_create("Hello World");
    printf("%s\n", string->value);
    string = string_append_character(string, '!');
    printf("%s\n", string->value);
    
    array_list_t* array_list = array_list_create();
    
    string_t *string_1 = string_create("1");
    string_t *string_2 = string_create("2");
    string_t *string_3 = string_create("3");
    array_list_add(array_list, 0, string_1);
    array_list_add(array_list, 0, string_2);
    array_list_add(array_list, 0, string_3);
    
    string_t *display_string = string_create("Counting: {}, {}, {}");
    string_t* format_string = string_format(display_string, array_list);
    printf("%s|len=%d\n", format_string->value, format_string->length);

    string_deallocate(display_string);
    string_deallocate(format_string);
    string_deallocate(string);
    string_deallocate(string_1);
    string_deallocate(string_2);
    string_deallocate(string_3);
    array_list_deallocate(array_list);
    
    return 0;
}

void compile(int argument_count, char** argument_variables) {
    for (size_t i = 1; i < argument_count; i++) {
        string_t *file_name = string_create(argument_variables[i]);
        verilog_file_stream_reader_t* verilog_file_stream_reader = create_verilog_file_stream_reader(file_name);

        printf("%s\n", file_name->value);
        while (verilog_reader_has_next_char(verilog_file_stream_reader)) {
            char ch = verilog_read_next_char(verilog_file_stream_reader);
            printf("%c", ch);
        }

        string_deallocate(file_name);
        free_verilog_file_stream_reader(verilog_file_stream_reader);
    }
}
