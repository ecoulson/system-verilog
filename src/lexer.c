#include <stdlib.h>

#include "lexer.h"
#include "verilog_file_stream_reader.h"

lexer_t* lexer_create(verilog_file_stream_reader_t* file_stream_reader) {
    lexer_t* lexer = malloc(sizeof(lexer_t));

    lexer->file_stream_reader = file_stream_reader;

    return lexer;
}

void lexer_deallocate(lexer_t* lexer) {
    free(lexer);
}

char lexer_read_next_character(lexer_t* lexer) {
    return verilog_read_char(lexer->file_stream_reader);
}

char lexer_peek_next_character(lexer_t* lexer) {
    return verilog_peek_char(lexer->file_stream_reader);
}

token_t* lexer_create_error_token(lexer_t* lexer) {
    return token_create(
        TOKEN_ERROR,
        string_create(""),
        source_code_position_copy(lexer->file_stream_reader->source_code_position)
    );
}

int lexer_is_whitespace(char ch) {
    return ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t';
}

int lexer_is_punctuation(char ch) {
    return ch == ';' || ch == '$' || ch == '.' || 
            ch == '(' || ch == ')' || ch == ',' || 
            ch == ':' || ch == '*' || ch == '+' || 
            ch == '/' || ch == '#' || ch == '=' ||
            ch == '[' || ch == ']' || ch == '&' ||
            ch == '^' || ch == '|' || ch == '>' ||
            ch == '<' || ch == '{' || ch == '}' ||
            ch == '!' || ch == '%' || ch == '@' ||
            ch == '?' || ch == '~' || ch == '_' ||
            ch == '-'|| ch == '\'';
}

token_type_t lexer_get_punctuation_type(char punctuation) {
    switch (punctuation)
    {
        case ';':
            return TOKEN_SEMI_COLON;
        case '$':
            return TOKEN_DOLLOR_SIGN;
        case '(':
            return TOKEN_LEFT_PARENTHESE;
        case ')':
            return TOKEN_RIGHT_PARENTHESE;
        case '.':
            return TOKEN_PERIOD;
        case '_':
            return TOKEN_UNDERSCORE;
        case '-':
            return TOKEN_DASH;
        case ',':
            return TOKEN_COMMA;
        case ':':
            return TOKEN_COLON;
        case '*':
            return TOKEN_ASTRIX;
        case '+':
            return TOKEN_PLUS;
        case '/':
            return TOKEN_SLASH;
        case '#':
            return TOKEN_POUND;
        case '=':
            return TOKEN_EQUAL;
        case '[':
            return TOKEN_LEFT_BRACKET;
        case ']':
            return TOKEN_RIGHT_BRACKET;
        case '&':
            return TOKEN_AMPERSAND;
        case '^':
            return TOKEN_CAROT;
        case '|':
            return TOKEN_PIPE;
        case '<':
            return TOKEN_LEFT_ANGLE_BRACKET;
        case '>':
            return TOKEN_RIGHT_ANGLE_BRACKET;
        case '{':
            return TOKEN_LEFT_CURLY_BRACE;
        case '}':
            return TOKEN_RIGHT_CURLY_BRACE;
        case '!':
            return TOKEN_EXCLAMATION_MARK;
        case '%':
            return TOKEN_PERCENT;
        case '@':
            return TOKEN_ASPERAND;
        case '?':
            return TOKEN_QUESTION_MARK;
        case '~':
            return TOKEN_TILDE;
        case '\'':
            return TOKEN_QUOTATION_MARK;
        case '\\':
            return TOKEN_BACKSLASH;
        case '"':
            return TOKEN_DOUBLE_QUOTATION_MARK;
        case '`':
            return TOKEN_BACK_TICK;
        default:
            return TOKEN_UNKNOWN;
    }
}

void lexer_read_whitespace(lexer_t* lexer) {
    char ch = lexer_peek_next_character(lexer);

    while (lexer_is_whitespace(ch)) {
        lexer_read_next_character(lexer);
        ch = lexer_peek_next_character(lexer);
    }
}

int lexer_is_string(char ch) {
    return ch == '"';
}

int lexer_is_escape_character(char ch) {
    return ch == '\\';
}

token_t* lexer_read_string(lexer_t* lexer, source_code_position_t* source_code_position) {
    string_t* string = string_create("");
    char ch = lexer_peek_next_character(lexer);

    if (!lexer_is_string(ch)) {
        return lexer_create_error_token(lexer);
    }

    string = string_append_character(string, lexer_read_next_character(lexer));
    ch = lexer_peek_next_character(lexer);

    while (!lexer_is_string(ch) && ch != EOF) {
        if (lexer_is_escape_character(ch)) {
            string = string_append_character(string, lexer_read_next_character(lexer));
            ch = lexer_peek_next_character(lexer);
        }

        string = string_append_character(string, lexer_read_next_character(lexer));
        ch = lexer_peek_next_character(lexer);
    }

    if (lexer_is_string(ch)) {
        string = string_append_character(string, lexer_read_next_character(lexer));
    }

    return token_create(TOKEN_CHARACTER_SEQUENCE, string, source_code_position);
}

token_t* lexer_read_punctuation(lexer_t* lexer, source_code_position_t* position) {
    char next_char = lexer_read_next_character(lexer);
    int type = lexer_get_punctuation_type(next_char);

    if (type == TOKEN_UNKNOWN) {
        return lexer_create_error_token(lexer);
    }

    string_t* punctuation = string_create("");
    punctuation = string_append_character(punctuation, next_char);

    return token_create(type, punctuation, position);
}

int lexer_is_number_sequence(char ch) {
    return '0' <= ch && ch <= '9';
}

token_t* lexer_read_number_squence(lexer_t* lexer, source_code_position_t* position) {
    string_t* number = string_create("");
    char ch = lexer_peek_next_character(lexer);

    if (!lexer_is_number_sequence(ch)) {
        return lexer_create_error_token(lexer);
    }

    while (lexer_is_number_sequence(ch)) {
        number = string_append_character(number, lexer_read_next_character(lexer));
        ch = lexer_peek_next_character(lexer);
    }

    return token_create(TOKEN_NUMBER_SEQUENCE, number, position);
}

int lexer_is_letter(char ch) {
    return ('a' <= ch && ch <= 'z') ||
            ('A' <= ch && ch <= 'Z');
}

token_t* lexer_read_char_sequence(lexer_t* lexer, source_code_position_t* position) {
    string_t* sequence = string_create("");
    char ch = lexer_peek_next_character(lexer);

    if (!lexer_is_letter(ch)) {
        return lexer_create_error_token(lexer);
    }

    while(lexer_is_letter(ch)) {
        sequence = string_append_character(sequence, lexer_read_next_character(lexer));
        ch = lexer_peek_next_character(lexer);
    }

    return token_create(TOKEN_CHARACTER_SEQUENCE, sequence, position);
}

int lexer_has_tokens_to_lex(lexer_t* lexer) {
    return lexer_peek_next_character(lexer) != EOF;
}

token_t* lexer_create_eof_token(lexer_t* lexer) {
    string_t* eof = string_create("");
    eof = string_append_character(eof, EOF);
    source_code_position_t* position = source_code_position_copy(
        lexer->file_stream_reader->source_code_position
    );
    return token_create(TOKEN_EOF, eof, position);
}

void lexer_seek_to(lexer_t* lexer, int seek_position) {
    verilog_seek_to(lexer->file_stream_reader, seek_position);
}

long lexer_get_current_seek_position(lexer_t* lexer) {
    return lexer->file_stream_reader->seek_position;
}

token_t* lexer_lex(lexer_t* lexer) {
    lexer_read_whitespace(lexer);

    if (!lexer_has_tokens_to_lex(lexer)) {
        return lexer_create_eof_token(lexer);
    }

    source_code_position_t* position = source_code_position_copy(
        lexer->file_stream_reader->source_code_position
    );
    token_t* possible_tokens[4];
    long possible_seek_positions[4];
    int current_seek_position = lexer_get_current_seek_position(lexer);

    possible_tokens[0] = lexer_read_string(lexer, position);
    possible_seek_positions[0] = lexer_get_current_seek_position(lexer);
    lexer_seek_to(lexer, current_seek_position);

    possible_tokens[1] = lexer_read_number_squence(lexer, position);
    possible_seek_positions[1] = lexer_get_current_seek_position(lexer);
    lexer_seek_to(lexer, current_seek_position);

    possible_tokens[2] = lexer_read_punctuation(lexer, position);
    possible_seek_positions[2] = lexer_get_current_seek_position(lexer);
    lexer_seek_to(lexer, current_seek_position);

    possible_tokens[3] = lexer_read_char_sequence(lexer, position);
    possible_seek_positions[3] = lexer_get_current_seek_position(lexer);
    lexer_seek_to(lexer, current_seek_position);
    
    int longest_match_index = -1;
    
    for (size_t i = 0; i < 4; i++) {
        token_t* matched_token = possible_tokens[i];

        if (longest_match_index == -1) {
            longest_match_index = i;
        }

        if (matched_token->type == TOKEN_ERROR) {
            continue;
        }

        token_t* longest_match = possible_tokens[longest_match_index];

        if (longest_match->value->length < matched_token->value->length) {
            longest_match_index = i;
        }
    }

    token_t* longest_match = possible_tokens[longest_match_index];
    long new_seek_position = possible_seek_positions[longest_match_index];
    lexer_seek_to(lexer, new_seek_position);

    return longest_match;
}
