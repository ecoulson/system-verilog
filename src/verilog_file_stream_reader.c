#include <stdlib.h>
#include "verilog_file_stream_reader.h"

verilog_file_stream_reader_t* create_verilog_file_stream_reader(string_t* path) {
    verilog_file_stream_reader_t* reader = malloc(sizeof(verilog_file_stream_reader_t));

    reader->file_path = path;
    reader->file = fopen(path->value, "r");
    reader->seek_position = 0;
    reader->source_code_position = create_source_code_position();

    return reader;
}

void free_verilog_file_stream_reader(verilog_file_stream_reader_t* verilog_file_stream_reader) {
    fclose(verilog_file_stream_reader->file);

    free_source_code_position(verilog_file_stream_reader->source_code_position);
    free(verilog_file_stream_reader);
}

char verilog_read_next_char(verilog_file_stream_reader_t* verilog_file_stream_reader) {
    if (!verilog_reader_has_next_char(verilog_file_stream_reader)) {
        printf("Trying to read outside of bounds of %s\n", verilog_file_stream_reader->file_path->value);
        exit(1);
        return EOF;
    }
    char ch = fgetc(verilog_file_stream_reader->file);
    verilog_file_stream_reader->seek_position++;
    update_source_code_position(verilog_file_stream_reader->source_code_position, ch);
    return ch;
}

int verilog_reader_has_next_char(verilog_file_stream_reader_t* verilog_file_stream_reader) {
    char ch = fgetc(verilog_file_stream_reader->file);
    ungetc(ch, verilog_file_stream_reader->file);
    return ch != EOF;
}

void print_verilog_file_stream_reader(FILE* stream, verilog_file_stream_reader_t* verilog_file_stream_reader) {
    fprintf(
        stream,
        "VerilogFileStreamReader {\n\tpath: %s\n\tseek_position: %d\n\tcolumn: %d\n\trow: %d\n}\n",
        verilog_file_stream_reader->file_path->value,
        verilog_file_stream_reader->seek_position,
        verilog_file_stream_reader->source_code_position->column,
        verilog_file_stream_reader->source_code_position->row
    );
}
