#include <stdlib.h>
#include <stdio.h>

#include "string.h"
#include "array_list.h"
#include "verilog_file_stream_reader.h"

int main(int argument_count, char** argument_variables) {
    array_list_t* list = array_list_create();

    for (size_t i = 0; i < 1000; i++) {
        int *value = (int*)array_list_add(list, i, (void*)i);
        value = array_list_get(list, i);
        printf("%d ", *value);
    }
    printf("\n");

    for (size_t i = 0; i < 1000; i++) {
        size_t updated_value = 8;
        array_list_set(list, i, (void*)updated_value);
        int* element = array_list_get(list, i);
        printf("%d ", *element);
    }
    printf("\n");

    for (int i = 999; i >= 0; i--) {
        array_list_remove(list, i);
    }
    printf("Removed all elements.\n");

    printf("Length: %d Capacity: %d\n", list->length, list->capacity);
    printf("Expected Length: 0, Capacity: 1\n");

    array_list_deallocate(list);

    return 0;
}

void compile(int argument_count, char** argument_variables) {
    for (size_t i = 1; i < argument_count; i++) {
        string_t *file_name = create_string(argument_variables[i]);
        verilog_file_stream_reader_t* verilog_file_stream_reader = create_verilog_file_stream_reader(file_name);

        print_string(stdout, file_name);
        while (verilog_reader_has_next_char(verilog_file_stream_reader)) {
            char ch = verilog_read_next_char(verilog_file_stream_reader);
            printf("%c", ch);
        }

        free_string(file_name);
        free_verilog_file_stream_reader(verilog_file_stream_reader);
    }
}
