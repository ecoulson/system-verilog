#ifndef FILE_STREAM_READER_H
#define FILE_STREAM_READER_H
#include <stdio.h>

#include "source_code_position.h"
#include "string.h"

typedef int seek_position_t;

typedef struct verilog_file_stream_reader_s {
    FILE* file;
    string_t* file_path;
    source_code_position_t* source_code_position;
    seek_position_t seek_position;
} verilog_file_stream_reader_t;

verilog_file_stream_reader_t* create_verilog_file_stream_reader(string_t* file_path);
void free_verilog_file_stream_reader(verilog_file_stream_reader_t* verilog_file_stream_reader);
char verilog_read_char(verilog_file_stream_reader_t* verilog_file_stream_reader);
char verilog_peek_char(verilog_file_stream_reader_t* verilog_file_stream_reader);
int verilog_reader_has_next_char(verilog_file_stream_reader_t* verilog_file_stream_reader);
void print_verilog_file_stream_reader(FILE* file, verilog_file_stream_reader_t* verilog_file_stream_reader);

#endif
