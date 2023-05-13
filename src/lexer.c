#include <stdlib.h>

#include "lexer.h"
#include "verilog_file_stream_reader.h"

lexer_t* lexer_create(verilog_file_stream_reader_t *file_stream_reader) {
    lexer_t* lexer = malloc(sizeof(lexer_t));

    lexer->file_stream_reader = file_stream_reader;

    return lexer;
}

void lexer_deallocate(lexer_t *lexer) {
    free(lexer);
}

int lexer_is_whitespace(char ch) {
    return ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t';
}

void lexer_read_whitespace(lexer_t* lexer) {
    char ch = verilog_peek_char(lexer->file_stream_reader);
    while (lexer_is_whitespace(ch)) {
        verilog_read_char(lexer->file_stream_reader);
        ch = verilog_peek_char(lexer->file_stream_reader);
    }
}

token_t* lexer_lex(lexer_t *lexer) {
    lexer_read_whitespace(lexer);

    string_t *value = string_create("");
    source_code_position_t* position = create_source_code_position();
    char ch = verilog_read_char(lexer->file_stream_reader);

    position->column = lexer->file_stream_reader->source_code_position->column;
    position->row = lexer->file_stream_reader->source_code_position->row;

    while(!lexer_is_whitespace(ch)) {
        value = string_append_character(value, ch);
        ch = verilog_read_char(lexer->file_stream_reader);
    }

    return token_create(0, value, position);
}

int lexer_has_tokens_to_lex(lexer_t *lexer) {
    return verilog_reader_has_next_char(lexer->file_stream_reader);
}

char* lexer_display(lexer_t* lexer) {
    return NULL;
}
