#ifndef FILE_STREAM_READER_H
#define FILE_STREAM_READER_H
#include <stdio.h>

#include "source_code_position.h"
#include "string.h"
#include "arena.h"

typedef struct verilog_file_stream_reader_s {
    FILE* file;
    source_code_position_t* source_code_position;
    long seek_position;
} verilog_file_stream_reader_t;

verilog_file_stream_reader_t* create_verilog_file_stream_reader(arena_t* arena, string_t* file_path);
char verilog_read_char(verilog_file_stream_reader_t* verilog_file_stream_reader);
void verilog_seek_to(verilog_file_stream_reader_t* verilog_file_stream_reader, int seek_position);
char verilog_peek_char(verilog_file_stream_reader_t* verilog_file_stream_reader);
int verilog_reader_has_next_char(verilog_file_stream_reader_t* verilog_file_stream_reader);
void print_verilog_file_stream_reader(FILE* file, verilog_file_stream_reader_t* verilog_file_stream_reader);

#endif
