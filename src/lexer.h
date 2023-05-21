#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "verilog_file_stream_reader.h"

typedef struct lexer_s {
    verilog_file_stream_reader_t* file_stream_reader;
} lexer_t;

lexer_t* lexer_create(verilog_file_stream_reader_t* file_stream_reader); 
void lexer_deallocate(lexer_t* lexer);
token_t* lexer_lex(lexer_t* token); 
int lexer_has_tokens_to_lex(lexer_t* lexer);

#endif
