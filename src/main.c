#include <stdlib.h>
#include <stdio.h>

#include "string.h"
#include "lexer.h"
#include "token.h"
#include "verilog_file_stream_reader.h"

void compile(int argument_count, char** argument_variables) {
    for (size_t i = 1; i < argument_count; i++) {
        string_t *file_name = string_create(argument_variables[i]);
        verilog_file_stream_reader_t* verilog_file_stream_reader = create_verilog_file_stream_reader(file_name);
        lexer_t* lexer = lexer_create(verilog_file_stream_reader);

        printf("%s\n", file_name->value);
        while (lexer_has_tokens_to_lex(lexer)) {
            token_t* token = lexer_lex(lexer);
            printf("Token: %s %s\n", token->value->value, "");
            token_deallocate(token);
        }

        string_deallocate(file_name);
        lexer_deallocate(lexer);
        free_verilog_file_stream_reader(verilog_file_stream_reader);
    }
}

int main(int argument_count, char** argument_variables) {
    compile(argument_count, argument_variables);
    
    return 0;
}
