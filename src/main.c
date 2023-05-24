#include <stdlib.h>
#include <stdio.h>

#include "string.h"
#include "lexer.h"
#include "token.h"
#include "arena.h"
#include "verilog_file_stream_reader.h"
#include "token_stream.h"

token_stream_t* lex_file(arena_t* arena, char* file_path) {
    string_t *file_name = string_create(arena, file_path);
    verilog_file_stream_reader_t* verilog_file_stream_reader = verilog_file_stream_reader_create(arena, file_name);
    lexer_t* lexer = lexer_create(arena, verilog_file_stream_reader);
    token_stream_t* token_stream = token_stream_create(arena);

    while (lexer_has_tokens_to_lex(lexer)) {
        token_t* token = lexer_lex(lexer);
        token_stream_write(token_stream, token);
    }

    lexer_close(lexer);

    return token_stream;
}

void compile(int argument_count, char** argument_variables) {
    // the parser then takes the token stream and creates a parse tree? Need to think
    // about how to link all the files together? Maybe it just creates the parse trees
    // and if certain things don't exist it assumes it could just be defined in another
    // file. Then it will produce the AST and write it out to a file? Then if it's not
    // defined in that file or any of the dependant trees based of includes then
    // determine the something is undefined

    for (size_t i = 1; i < argument_count; i++) {
        arena_t* lex_arena = arena_create(4000000); // 4mB

        token_stream_t* token_stream = lex_file(lex_arena, argument_variables[i]);

        while (token_stream_can_read(token_stream)) {
            token_t* token = token_stream_peek(token_stream);
            printf("%d %s\n", token->type, token->value->value);
            token_stream_read(token_stream);
        }

        // syntax_node_t* ast = parse_token_stream(token_stream);
        arena_free(lex_arena);
    }
}

int main(int argument_count, char** argument_variables) {
    compile(argument_count, argument_variables);
    
    return 0;
}
