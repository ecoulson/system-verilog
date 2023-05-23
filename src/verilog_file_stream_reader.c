#include <stdlib.h>
#include "verilog_file_stream_reader.h"

verilog_file_stream_reader_t* verilog_file_stream_reader_create(arena_t* arena, string_t* file_path) {
    verilog_file_stream_reader_t* reader = arena_allocate(arena, sizeof(verilog_file_stream_reader_t));

    reader->file = fopen(file_path->value, "r");
    reader->seek_position = 0;
    reader->source_code_position = source_code_position_create(arena, file_path);

    return reader;
}

char verilog_file_stream_reader_read_char(verilog_file_stream_reader_t* verilog_file_stream_reader) {
    if (!verilog_file_stream_reader_has_next_char(verilog_file_stream_reader)) {
        return EOF;
    }

    char ch = fgetc(verilog_file_stream_reader->file);
    verilog_file_stream_reader->seek_position++;
    source_code_position_move(verilog_file_stream_reader->source_code_position, ch);

    return ch;
}

char verilog_file_stream_reader_peek_char(verilog_file_stream_reader_t* verilog_file_stream_reader) {
    char ch = fgetc(verilog_file_stream_reader->file);
    ungetc(ch, verilog_file_stream_reader->file);

    return ch;
}

int verilog_file_stream_reader_has_next_char(verilog_file_stream_reader_t* verilog_file_stream_reader) {
    char ch = fgetc(verilog_file_stream_reader->file);
    ungetc(ch, verilog_file_stream_reader->file);

    return ch != EOF;
}

void verilog_file_stream_reader_seek_to(verilog_file_stream_reader_t* verilog_file_stream_reader, int seek_position) {
    fseek(verilog_file_stream_reader->file, seek_position, SEEK_SET);
    verilog_file_stream_reader->seek_position = seek_position;
}
