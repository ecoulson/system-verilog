#include <stdlib.h>
#include "verilog_file_stream_reader.h"

verilog_file_stream_reader_t* create_verilog_file_stream_reader(arena_t* arena, string_t* file_path) {
    verilog_file_stream_reader_t* reader = arena_allocate(arena, sizeof(verilog_file_stream_reader_t));

    reader->file = fopen(file_path->value, "r");
    reader->seek_position = 0;
    reader->source_code_position = create_source_code_position(arena, file_path);

    return reader;
}

char verilog_read_char(verilog_file_stream_reader_t* verilog_file_stream_reader) {
    if (!verilog_reader_has_next_char(verilog_file_stream_reader)) {
        return EOF;
    }

    char ch = fgetc(verilog_file_stream_reader->file);
    verilog_file_stream_reader->seek_position++;
    update_source_code_position(verilog_file_stream_reader->source_code_position, ch);

    return ch;
}

char verilog_peek_char(verilog_file_stream_reader_t* verilog_file_stream_reader) {
    char ch = fgetc(verilog_file_stream_reader->file);
    ungetc(ch, verilog_file_stream_reader->file);

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
        "VerilogFileStreamReader {\n\tpath: %s\n\tseek_position: %lu\n\tcolumn: %d\n\trow: %d\n}\n",
        verilog_file_stream_reader->source_code_position->file_path->value,
        verilog_file_stream_reader->seek_position,
        verilog_file_stream_reader->source_code_position->column,
        verilog_file_stream_reader->source_code_position->row
    );
}

void verilog_seek_to(verilog_file_stream_reader_t* verilog_file_stream_reader, int seek_position) {
    fseek(verilog_file_stream_reader->file, seek_position, SEEK_SET);
    verilog_file_stream_reader->seek_position = seek_position;
}
