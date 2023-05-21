#ifndef TOKEN_H
#define TOKEN_H

#include "string.h"
#include "source_code_position.h"

typedef struct token_s {
    int type;
    string_t* value;
    source_code_position_t* position;
} token_t;

typedef enum token_type_e {
    TOKEN_UNKNOWN = -1,
    TOKEN_IDENTIFIER = 1,
    TOKEN_LEFT_PARENTHESE = 2,
    TOKEN_RIGHT_PARENTHESE = 3,
    TOKEN_DOLLOR_SIGN = 4,
    TOKEN_SEMI_COLON = 5,
    TOKEN_PERIOD = 6,
    TOKEN_CHARACTER_SEQUENCE = 7,
    TOKEN_NUMBER_SEQUENCE = 8,
    TOKEN_COMMA = 9,
    TOKEN_ACCEPT_ON = 10,
    TOKEN_EOF = 11,
    TOKEN_ERROR = 12,
    TOKEN_UNDERSCORE = 13,
    TOKEN_DASH = 14,
    TOKEN_COLON = 15,
    TOKEN_ASTRIX = 16,
    TOKEN_PLUS = 17,
    TOKEN_SLASH = 18,
    TOKEN_POUND = 19,
    TOKEN_EQUAL = 20,
    TOKEN_LEFT_BRACKET = 21,
    TOKEN_RIGHT_BRACKET = 22,
    TOKEN_AMPERSAND = 23,
    TOKEN_CAROT = 24,
    TOKEN_PIPE = 25,
    TOKEN_LEFT_ANGLE_BRACKET = 26,
    TOKEN_RIGHT_ANGLE_BRACKET = 27,
    TOKEN_LEFT_CURLY_BRACE = 28,
    TOKEN_RIGHT_CURLY_BRACE = 29,
    TOKEN_EXCLAMATION_MARK = 30,
    TOKEN_PERCENT = 31,
    TOKEN_ASPERAND = 32,
    TOKEN_QUESTION_MARK = 33,
    TOKEN_TILDE = 34,
    TOKEN_BACKSLASH = 35,
    TOKEN_QUOTATION_MARK = 36,
    TOKEN_DOUBLE_QUOTATION_MARK = 37,
    TOKEN_BACK_TICK = 38
} token_type_t;


token_t* token_create(token_type_t type, string_t* value, source_code_position_t* source_code_position);
void token_deallocate(token_t* token); 

#endif
