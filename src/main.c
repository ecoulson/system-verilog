#include <stdlib.h>
#include <stdio.h>

#include "string.h"
#include "verilog_file_stream_reader.h"

int main(int argument_count, char** argument_variables) {
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
    return 0;
}
