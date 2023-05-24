#include <stdlib.h>
#include <stdio.h>

#include "string.h"
#include "lexer.h"
#include "token.h"
#include "arena.h"
#include "verilog_file_stream_reader.h"

void compile(int argument_count, char** argument_variables) {
    arena_t* arena = arena_create(4000000); // 4mB

    for (size_t i = 1; i < argument_count; i++) {
        string_t *file_name = string_create(arena, argument_variables[i]);
        verilog_file_stream_reader_t* verilog_file_stream_reader = verilog_file_stream_reader_create(arena, file_name);
        lexer_t* lexer = lexer_create(arena, verilog_file_stream_reader);

        while (lexer_has_tokens_to_lex(lexer)) {
            token_t* token = lexer_lex(lexer);
            printf(
                "Token: %d %s %s\n", 
                token->type, 
                token->value->value,
                lexer->file_stream_reader->source_code_position->file_path->value
            );
        }

        lexer_close(lexer);
    }

    arena_free(arena);
}

int main(int argument_count, char** argument_variables) {
    compile(argument_count, argument_variables);
    
    return 0;
}
