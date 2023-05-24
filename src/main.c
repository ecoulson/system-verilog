#include <stdlib.h>
#include <stdio.h>

#include "string.h"
#include "lexer.h"
#include "token.h"
#include "arena.h"
#include "verilog_file_stream_reader.h"

void compile(int argument_count, char** argument_variables) {
    // TODO: token stream (this should be a struct that creates an array list)   
    // it should have operations for reading the next token, asserting the next token
    // peeking the next token, checking for remaining tokens, writing a token to the stream

    // the parser then takes the token stream and creates a parse tree? Need to think
    // about how to link all the files together? Maybe it just creates the parse trees
    // and if certain things don't exist it assumes it could just be defined in another
    // file. Then it will produce the AST and write it out to a file? Then if it's not
    // defined in that file or any of the dependant trees based of includes then
    // determine the something is undefined

    for (size_t i = 1; i < argument_count; i++) {
        arena_t* arena = arena_create(4000000); // 4mB
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
        arena_free(arena);
    }
}

int main(int argument_count, char** argument_variables) {
    compile(argument_count, argument_variables);
    
    return 0;
}
