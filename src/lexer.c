#include <stdlib.h>

#include "lexer.h"
#include "verilog_file_stream_reader.h"

#define LEXER_OPERATION_COUNT 5

typedef enum lexer_read_type_e {
    LEXER_READ_STRING,
    LEXER_READ_NUMBER_SEQUENCE,
    LEXER_READ_PUNCTUATION,
    LEXER_READ_CHARACTER_SEQUENCE,
    LEXER_READ_KEYWORD,
} lexer_read_type_t;

lexer_t* lexer_create(arena_t* arena, verilog_file_stream_reader_t* file_stream_reader) {
    lexer_t* lexer = arena_allocate(arena, sizeof(lexer_t));

    lexer->arena = arena;
    lexer->file_stream_reader = file_stream_reader;

    return lexer;
}

char lexer_read_next_character(lexer_t* lexer) {
    return verilog_file_stream_reader_read_char(lexer->file_stream_reader);
}

char lexer_peek_next_character(lexer_t* lexer) {
    return verilog_file_stream_reader_peek_char(lexer->file_stream_reader);
}

token_t* lexer_create_error_token(lexer_t* lexer) {
    return token_create(
        lexer->arena,
        TOKEN_ERROR,
        string_create(lexer->arena, ""),
        source_code_position_copy(lexer->arena, lexer->file_stream_reader->source_code_position)
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
    string_t* string = string_create(lexer->arena, "");
    char ch = lexer_peek_next_character(lexer);

    if (!lexer_is_string(ch)) {
        return lexer_create_error_token(lexer);
    }

    string = string_append_character(lexer->arena, string, lexer_read_next_character(lexer));
    ch = lexer_peek_next_character(lexer);

    while (!lexer_is_string(ch) && ch != EOF) {
        if (lexer_is_escape_character(ch)) {
            string = string_append_character(lexer->arena, string, lexer_read_next_character(lexer));
            ch = lexer_peek_next_character(lexer);
        }

        string = string_append_character(lexer->arena, string, lexer_read_next_character(lexer));
        ch = lexer_peek_next_character(lexer);
    }

    if (lexer_is_string(ch)) {
        string = string_append_character(lexer->arena, string, lexer_read_next_character(lexer));
    }

    return token_create(lexer->arena, TOKEN_CHARACTER_SEQUENCE, string, source_code_position);
}

token_t* lexer_read_punctuation(lexer_t* lexer, source_code_position_t* position) {
    char next_char = lexer_read_next_character(lexer);
    int type = lexer_get_punctuation_type(next_char);

    if (type == TOKEN_UNKNOWN) {
        return lexer_create_error_token(lexer);
    }

    string_t* punctuation = string_create(lexer->arena, "");
    punctuation = string_append_character(lexer->arena, punctuation, next_char);

    return token_create(lexer->arena, type, punctuation, position);
}

int lexer_is_number_sequence(char ch) {
    return '0' <= ch && ch <= '9';
}

token_t* lexer_read_number_squence(lexer_t* lexer, source_code_position_t* position) {
    string_t* number = string_create(lexer->arena, "");
    char ch = lexer_peek_next_character(lexer);

    if (!lexer_is_number_sequence(ch)) {
        return lexer_create_error_token(lexer);
    }

    while (lexer_is_number_sequence(ch)) {
        number = string_append_character(lexer->arena, number, lexer_read_next_character(lexer));
        ch = lexer_peek_next_character(lexer);
    }

    return token_create(lexer->arena, TOKEN_NUMBER_SEQUENCE, number, position);
}

int lexer_is_letter(char ch) {
    return ('a' <= ch && ch <= 'z') ||
            ('A' <= ch && ch <= 'Z');
}

token_t* lexer_read_char_sequence(lexer_t* lexer, source_code_position_t* position) {
    string_t* sequence = string_create(lexer->arena, "");
    char ch = lexer_peek_next_character(lexer);

    if (!lexer_is_letter(ch)) {
        return lexer_create_error_token(lexer);
    }

    while(lexer_is_letter(ch)) {
        sequence = string_append_character(lexer->arena, sequence, lexer_read_next_character(lexer));
        ch = lexer_peek_next_character(lexer);
    }

    return token_create(lexer->arena, TOKEN_CHARACTER_SEQUENCE, sequence, position);
}

token_type_t lexer_get_keyword_type(string_t* character_sequence) {
    if (string_equals_constant(character_sequence, "accept_on")) {
        return TOKEN_ACCEPT_ON;
    }

    if (string_equals_constant(character_sequence, "alias")) {
        return TOKEN_ALIAS;
    }

    if (string_equals_constant(character_sequence, "always")) {
        return TOKEN_ALWAYS;
    }

    if (string_equals_constant(character_sequence, "always_comb")) {
        return TOKEN_ALWAYS_COMB;
    }

    if (string_equals_constant(character_sequence, "always_ff")) {
        return TOKEN_ALWAYS_FF;
    }

    if (string_equals_constant(character_sequence, "always_latch")) {
        return TOKEN_ALWAYS_LATCH;
    }

    if (string_equals_constant(character_sequence, "and")) {
        return TOKEN_AND;
    }

    if (string_equals_constant(character_sequence, "assert")) {
        return TOKEN_ASSERT;
    }

    if (string_equals_constant(character_sequence, "assign")) {
        return TOKEN_ASSIGN;
    }

    if (string_equals_constant(character_sequence, "assume")) {
        return TOKEN_ASSUME;
    }

    if (string_equals_constant(character_sequence, "automatic")) {
        return TOKEN_AUTOMATIC;
    }

    if (string_equals_constant(character_sequence, "before")) {
        return TOKEN_BEFORE;
    }

    if (string_equals_constant(character_sequence, "begin")) {
        return TOKEN_BEGIN;
    }

    if (string_equals_constant(character_sequence, "bind")) {
        return TOKEN_BIND;
    }

    if (string_equals_constant(character_sequence, "bins")) {
        return TOKEN_BINS;
    }

    if (string_equals_constant(character_sequence, "binsof")) {
        return TOKEN_BINSOF;
    }

    if (string_equals_constant(character_sequence, "bit")) {
        return TOKEN_BIT;
    }

    if (string_equals_constant(character_sequence, "break")) {
        return TOKEN_BREAK;
    }

    if (string_equals_constant(character_sequence, "buf")) {
        return TOKEN_BUF;
    }

    if (string_equals_constant(character_sequence, "bufif0")) {
        return TOKEN_BUFIF0;
    }

    if (string_equals_constant(character_sequence, "bufif1")) {
        return TOKEN_BUFIF1;
    }

    if (string_equals_constant(character_sequence, "byte")) {
        return TOKEN_BYTE;
    }

    if (string_equals_constant(character_sequence, "case")) {
        return TOKEN_CASE;
    }

    if (string_equals_constant(character_sequence, "casex")) {
        return TOKEN_CASEX;
    }

    if (string_equals_constant(character_sequence, "casez")) {
        return TOKEN_CASEZ;
    }

    if (string_equals_constant(character_sequence, "cell")) {
        return TOKEN_CELL;
    }

    if (string_equals_constant(character_sequence, "chandle")) {
        return TOKEN_CHANDLE;
    }

    if (string_equals_constant(character_sequence, "checker")) {
        return TOKEN_CHECKER;
    }

    if (string_equals_constant(character_sequence, "class")) {
        return TOKEN_CLASS;
    }

    if (string_equals_constant(character_sequence, "clocking")) {
        return TOKEN_CLOCKING;
    }

    if (string_equals_constant(character_sequence, "cmos")) {
        return TOKEN_CMOS;
    }

    if (string_equals_constant(character_sequence, "config")) {
        return TOKEN_CONFIG;
    }

    if (string_equals_constant(character_sequence, "const")) {
        return TOKEN_CONST;
    }

    if (string_equals_constant(character_sequence, "constraint")) {
        return TOKEN_CONSTRAINT;
    }

    if (string_equals_constant(character_sequence, "context")) {
        return TOKEN_CONTEXT;
    }

    if (string_equals_constant(character_sequence, "continue")) {
        return TOKEN_CONTINUE;
    }

    if (string_equals_constant(character_sequence, "cover")) {
        return TOKEN_COVER;
    }

    if (string_equals_constant(character_sequence, "covergroup")) {
        return TOKEN_COVERGROUP;
    }

    if (string_equals_constant(character_sequence, "coverpoint")) {
        return TOKEN_COVERPOINT;
    }

    if (string_equals_constant(character_sequence, "cross")) {
        return TOKEN_CROSS;
    }

    if (string_equals_constant(character_sequence, "deassign")) {
        return TOKEN_DEASSIGN;
    }

    if (string_equals_constant(character_sequence, "default")) {
        return TOKEN_DEFAULT;
    }

    if (string_equals_constant(character_sequence, "defparam")) {
        return TOKEN_DEFPARAM;
    }

    if (string_equals_constant(character_sequence, "design")) {
        return TOKEN_DESIGN;
    }

    if (string_equals_constant(character_sequence, "dist")) {
        return TOKEN_DIST;
    }

    if (string_equals_constant(character_sequence, "do")) {
        return TOKEN_DO;
    }

    if (string_equals_constant(character_sequence, "edge")) {
        return TOKEN_EDGE;
    }

    if (string_equals_constant(character_sequence, "else")) {
        return TOKEN_ELSE;
    }

    if (string_equals_constant(character_sequence, "end")) {
        return TOKEN_END;
    }

    if (string_equals_constant(character_sequence, "endcase")) {
        return TOKEN_ENDCASE;
    }

    if (string_equals_constant(character_sequence, "endchecker")) {
        return TOKEN_ENDCHECKER;
    }

    if (string_equals_constant(character_sequence, "endclass")) {
        return TOKEN_ENDCLASS;
    }

    if (string_equals_constant(character_sequence, "endclocking")) {
        return TOKEN_ENDCLOCKING;
    }

    if (string_equals_constant(character_sequence, "endconfig")) {
        return TOKEN_ENDCONFIG;
    }

    if (string_equals_constant(character_sequence, "endfunction")) {
        return TOKEN_ENDFUNCTION;
    }

    if (string_equals_constant(character_sequence, "endgenerate")) {
        return TOKEN_ENDGENERATE;
    }

    if (string_equals_constant(character_sequence, "endgroup")) {
        return TOKEN_ENDGROUP;
    }

    if (string_equals_constant(character_sequence, "endinterface")) {
        return TOKEN_ENDINTERFACE;
    }

    if (string_equals_constant(character_sequence, "endmodule")) {
        return TOKEN_ENDMODULE;
    }

    if (string_equals_constant(character_sequence, "endpackage")) {
        return TOKEN_ENDPACKAGE;
    }

    if (string_equals_constant(character_sequence, "endprimitive")) {
        return TOKEN_ENDPRIMITIVE;
    }

    if (string_equals_constant(character_sequence, "endprogram")) {
        return TOKEN_ENDPROGRAM;
    }

    if (string_equals_constant(character_sequence, "endproperty")) {
        return TOKEN_ENDPROPERTY;
    }

    if (string_equals_constant(character_sequence, "endspecify")) {
        return TOKEN_ENDSPECIFY;
    }

    if (string_equals_constant(character_sequence, "endsequence")) {
        return TOKEN_ENDSEQUENCE;
    }

    if (string_equals_constant(character_sequence, "endtable")) {
        return TOKEN_ENDTABLE;
    }

    if (string_equals_constant(character_sequence, "endtask")) {
        return TOKEN_ENDTASK;
    }

    if (string_equals_constant(character_sequence, "enum")) {
        return TOKEN_ENUM;
    }

    if (string_equals_constant(character_sequence, "event")) {
        return TOKEN_EVENT;
    }

    if (string_equals_constant(character_sequence, "eventually")) {
        return TOKEN_EVENTUALLY;
    }

    if (string_equals_constant(character_sequence, "expect")) {
        return TOKEN_EXPECT;
    }

    if (string_equals_constant(character_sequence, "export")) {
        return TOKEN_EXPORT;
    }

    if (string_equals_constant(character_sequence, "extends")) {
        return TOKEN_EXTENDS;
    }

    if (string_equals_constant(character_sequence, "extern")) {
        return TOKEN_EXTERN;
    }

    if (string_equals_constant(character_sequence, "final")) {
        return TOKEN_FINAL;
    }

    if (string_equals_constant(character_sequence, "first_match")) {
        return TOKEN_FIRST_MATCH;
    }

    if (string_equals_constant(character_sequence, "for")) {
        return TOKEN_FOR;
    }

    if (string_equals_constant(character_sequence, "force")) {
        return TOKEN_FORCE;
    }

    if (string_equals_constant(character_sequence, "foreach")) {
        return TOKEN_FOREACH;
    }

    if (string_equals_constant(character_sequence, "forever")) {
        return TOKEN_FOREVER;
    }

    if (string_equals_constant(character_sequence, "fork")) {
        return TOKEN_FORK;
    }

    if (string_equals_constant(character_sequence, "forkjoin")) {
        return TOKEN_FORKJOIN;
    }

    if (string_equals_constant(character_sequence, "function")) {
        return TOKEN_FUNCTION;
    }

    if (string_equals_constant(character_sequence, "generate")) {
        return TOKEN_GENERATE;
    }

    if (string_equals_constant(character_sequence, "genvar")) {
        return TOKEN_GENVAR;
    }

    if (string_equals_constant(character_sequence, "global")) {
        return TOKEN_GLOBAL;
    }

    if (string_equals_constant(character_sequence, "highz0")) {
        return TOKEN_HIGHZ0;
    }

    if (string_equals_constant(character_sequence, "highz1")) {
        return TOKEN_HIGHZ1;
    }

    if (string_equals_constant(character_sequence, "if")) {
        return TOKEN_IF;
    }

    if (string_equals_constant(character_sequence, "iff")) {
        return TOKEN_IFF;
    }

    if (string_equals_constant(character_sequence, "ifnone")) {
        return TOKEN_IFNONE;
    }

    if (string_equals_constant(character_sequence, "ignore_bins")) {
        return TOKEN_IGNORE_BINS;
    }

    if (string_equals_constant(character_sequence, "illegal_bins")) {
        return TOKEN_ILLEGAL_BINS;
    }

    if (string_equals_constant(character_sequence, "implements")) {
        return TOKEN_IMPLEMENTS;
    }

    if (string_equals_constant(character_sequence, "implies")) {
        return TOKEN_IMPLIES;
    }

    if (string_equals_constant(character_sequence, "import")) {
        return TOKEN_IMPORT;
    }

    if (string_equals_constant(character_sequence, "incdir")) {
        return TOKEN_INCDIR;
    }

    if (string_equals_constant(character_sequence, "include")) {
        return TOKEN_INCLUDE;
    }

    if (string_equals_constant(character_sequence, "initial")) {
        return TOKEN_INITIAL;
    }

    if (string_equals_constant(character_sequence, "inout")) {
        return TOKEN_INOUT;
    }

    if (string_equals_constant(character_sequence, "input")) {
        return TOKEN_INPUT;
    }

    if (string_equals_constant(character_sequence, "inside")) {
        return TOKEN_INSIDE;
    }

    if (string_equals_constant(character_sequence, "instance")) {
        return TOKEN_INSTANCE;
    }

    if (string_equals_constant(character_sequence, "int")) {
        return TOKEN_INT;
    }

    if (string_equals_constant(character_sequence, "integer")) {
        return TOKEN_INTEGER;
    }

    if (string_equals_constant(character_sequence, "interconnect")) {
        return TOKEN_INTERCONNECT;
    }

    if (string_equals_constant(character_sequence, "interface")) {
        return TOKEN_INTERFACE;
    }

    if (string_equals_constant(character_sequence, "intersect")) {
        return TOKEN_INTERSECT;
    }

    if (string_equals_constant(character_sequence, "join")) {
        return TOKEN_JOIN;
    }

    if (string_equals_constant(character_sequence, "join_any")) {
        return TOKEN_JOIN_ANY;
    }

    if (string_equals_constant(character_sequence, "join_none")) {
        return TOKEN_JOIN_NONE;
    }

    if (string_equals_constant(character_sequence, "large")) {
        return TOKEN_LARGE;
    }

    if (string_equals_constant(character_sequence, "let")) {
        return TOKEN_LET;
    }

    if (string_equals_constant(character_sequence, "liblist")) {
        return TOKEN_LIBLIST;
    }

    if (string_equals_constant(character_sequence, "library")) {
        return TOKEN_LIBRARY;
    }

    if (string_equals_constant(character_sequence, "local")) {
        return TOKEN_LOCAL;
    }

    if (string_equals_constant(character_sequence, "localparam")) {
        return TOKEN_LOCALPARAM;
    }

    if (string_equals_constant(character_sequence, "logic")) {
        return TOKEN_LOGIC;
    }

    if (string_equals_constant(character_sequence, "longint")) {
        return TOKEN_LONGINT;
    }

    if (string_equals_constant(character_sequence, "macromodule")) {
        return TOKEN_MACROMODULE;
    }

    if (string_equals_constant(character_sequence, "matches")) {
        return TOKEN_MATCHES;
    }

    if (string_equals_constant(character_sequence, "medium")) {
        return TOKEN_MEDIUM;
    }

    if (string_equals_constant(character_sequence, "modport")) {
        return TOKEN_MODPORT;
    }

    if (string_equals_constant(character_sequence, "module")) {
        return TOKEN_MODULE;
    }

    if (string_equals_constant(character_sequence, "nand")) {
        return TOKEN_NAND;
    }

    if (string_equals_constant(character_sequence, "negedge")) {
        return TOKEN_NEGEDGE;
    }

    if (string_equals_constant(character_sequence, "nettype")) {
        return TOKEN_NETTYPE;
    }

    if (string_equals_constant(character_sequence, "new")) {
        return TOKEN_NEW;
    }

    if (string_equals_constant(character_sequence, "nexttime")) {
        return TOKEN_NEXTTIME;
    }

    if (string_equals_constant(character_sequence, "nmos")) {
        return TOKEN_NMOS;
    }

    if (string_equals_constant(character_sequence, "nor")) {
        return TOKEN_NOR;
    }

    if (string_equals_constant(character_sequence, "noshowcancelled")) {
        return TOKEN_NOSHOWCANCELLED;
    }

    if (string_equals_constant(character_sequence, "not")) {
        return TOKEN_NOT;
    }

    if (string_equals_constant(character_sequence, "notif0")) {
        return TOKEN_NOTIF0;
    }

    if (string_equals_constant(character_sequence, "notif1")) {
        return TOKEN_NOTIF1;
    }

    if (string_equals_constant(character_sequence, "null")) {
        return TOKEN_NULL;
    }

    if (string_equals_constant(character_sequence, "or")) {
        return TOKEN_OR;
    }

    if (string_equals_constant(character_sequence, "output")) {
        return TOKEN_OUTPUT;
    }

    if (string_equals_constant(character_sequence, "package")) {
        return TOKEN_PACKAGE;
    }

    if (string_equals_constant(character_sequence, "packed")) {
        return TOKEN_PACKED;
    }

    if (string_equals_constant(character_sequence, "parameter")) {
        return TOKEN_PARAMETER;
    }

    if (string_equals_constant(character_sequence, "pmos")) {
        return TOKEN_PMOS;
    }

    if (string_equals_constant(character_sequence, "posedge")) {
        return TOKEN_POSEDGE;
    }

    if (string_equals_constant(character_sequence, "primitive")) {
        return TOKEN_PRIMITIVE;
    }

    if (string_equals_constant(character_sequence, "priority")) {
        return TOKEN_PRIORITY;
    }

    if (string_equals_constant(character_sequence, "program")) {
        return TOKEN_PROGRAM;
    }

    if (string_equals_constant(character_sequence, "property")) {
        return TOKEN_PROPERTY;
    }

    if (string_equals_constant(character_sequence, "protected")) {
        return TOKEN_PROTECTED;
    }

    if (string_equals_constant(character_sequence, "pull0")) {
        return TOKEN_PULL0;
    }

    if (string_equals_constant(character_sequence, "pull1")) {
        return TOKEN_PULL1;
    }

    if (string_equals_constant(character_sequence, "pulldown")) {
        return TOKEN_PULLDOWN;
    }

    if (string_equals_constant(character_sequence, "pullup")) {
        return TOKEN_PULLUP;
    }

    if (string_equals_constant(character_sequence, "pulsestyle_ondetect")) {
        return TOKEN_PULSESTYLE_ONDETECT;
    }

    if (string_equals_constant(character_sequence, "pulsestyle_onevent")) {
        return TOKEN_PULSESTYLE_ONEVENT;
    }

    if (string_equals_constant(character_sequence, "pure")) {
        return TOKEN_PURE;
    }

    if (string_equals_constant(character_sequence, "rand")) {
        return TOKEN_RAND;
    }

    if (string_equals_constant(character_sequence, "randc")) {
        return TOKEN_RANDC;
    }

    if (string_equals_constant(character_sequence, "randcase")) {
        return TOKEN_RANDCASE;
    }

    if (string_equals_constant(character_sequence, "randsequence")) {
        return TOKEN_RANDSEQUENCE;
    }

    if (string_equals_constant(character_sequence, "rcmos")) {
        return TOKEN_RCMOS;
    }

    if (string_equals_constant(character_sequence, "real")) {
        return TOKEN_REAL;
    }

    if (string_equals_constant(character_sequence, "realtime")) {
        return TOKEN_REALTIME;
    }

    if (string_equals_constant(character_sequence, "ref")) {
        return TOKEN_REF;
    }

    if (string_equals_constant(character_sequence, "reg")) {
        return TOKEN_REG;
    }

    if (string_equals_constant(character_sequence, "reject_on")) {
        return TOKEN_REJECT_ON;
    }

    if (string_equals_constant(character_sequence, "release")) {
        return TOKEN_RELEASE;
    }

    if (string_equals_constant(character_sequence, "repeat")) {
        return TOKEN_REPEAT;
    }

    if (string_equals_constant(character_sequence, "restrict")) {
        return TOKEN_RESTRICT;
    }

    if (string_equals_constant(character_sequence, "return")) {
        return TOKEN_RETURN;
    }

    if (string_equals_constant(character_sequence, "rnmos")) {
        return TOKEN_RNMOS;
    }

    if (string_equals_constant(character_sequence, "rpmos")) {
        return TOKEN_RPMOS;
    }

    if (string_equals_constant(character_sequence, "rtrain")) {
        return TOKEN_RTRAIN;
    }

    if (string_equals_constant(character_sequence, "rtrainif0")) {
        return TOKEN_RTRAINIF0;
    }

    if (string_equals_constant(character_sequence, "rtrainif1")) {
        return TOKEN_RTRAINIF1;
    }

    if (string_equals_constant(character_sequence, "s_always")) {
        return TOKEN_S_ALWAYS;
    }

    if (string_equals_constant(character_sequence, "s_eventually")) {
        return TOKEN_S_EVENTUALLY;
    }

    if (string_equals_constant(character_sequence, "s_nexttime")) {
        return TOKEN_S_NEXTTIME;
    }

    if (string_equals_constant(character_sequence, "s_until")) {
        return TOKEN_S_UNTIL;
    }

    if (string_equals_constant(character_sequence, "s_until_with")) {
        return TOKEN_S_UNTIL_WITH;
    }

    if (string_equals_constant(character_sequence, "scalared")) {
        return TOKEN_SCALARED;
    }

    if (string_equals_constant(character_sequence, "sequence")) {
        return TOKEN_SEQUENCE;
    }

    if (string_equals_constant(character_sequence, "shortint")) {
        return TOKEN_SHORTINT;
    }

    if (string_equals_constant(character_sequence, "shortreal")) {
        return TOKEN_SHORTREAL;
    }

    if (string_equals_constant(character_sequence, "showcancelled")) {
        return TOKEN_SHOWCANCELLED;
    }

    if (string_equals_constant(character_sequence, "signed")) {
        return TOKEN_SIGNED;
    }

    if (string_equals_constant(character_sequence, "small")) {
        return TOKEN_SMALL;
    }

    if (string_equals_constant(character_sequence, "soft")) {
        return TOKEN_SOFT;
    }

    if (string_equals_constant(character_sequence, "solve")) {
        return TOKEN_SOLVE;
    }

    if (string_equals_constant(character_sequence, "specify")) {
        return TOKEN_SPECIFY;
    }

    if (string_equals_constant(character_sequence, "specparam")) {
        return TOKEN_SPECPARAM;
    }

    if (string_equals_constant(character_sequence, "static")) {
        return TOKEN_STATIC;
    }

    if (string_equals_constant(character_sequence, "string")) {
        return TOKEN_STRING;
    }

    if (string_equals_constant(character_sequence, "strong0")) {
        return TOKEN_STRONG0;
    }

    if (string_equals_constant(character_sequence, "strong1")) {
        return TOKEN_STRONG1;
    }

    if (string_equals_constant(character_sequence, "struct")) {
        return TOKEN_STRUCT;
    }

    if (string_equals_constant(character_sequence, "super")) {
        return TOKEN_SUPER;
    }

    if (string_equals_constant(character_sequence, "supply0")) {
        return TOKEN_SUPPLY0;
    }

    if (string_equals_constant(character_sequence, "supply1")) {
        return TOKEN_SUPPLY1;
    }

    if (string_equals_constant(character_sequence, "sync_accept_on")) {
        return TOKEN_SYNC_ACCEPT_ON;
    }

    if (string_equals_constant(character_sequence, "sync_reject_on")) {
        return TOKEN_SYNC_REJECT_ON;
    }

    if (string_equals_constant(character_sequence, "table")) {
        return TOKEN_TABLE;
    }

    if (string_equals_constant(character_sequence, "tagged")) {
        return TOKEN_TAGGED;
    }

    if (string_equals_constant(character_sequence, "task")) {
        return TOKEN_TASK;
    }

    if (string_equals_constant(character_sequence, "this")) {
        return TOKEN_THIS;
    }

    if (string_equals_constant(character_sequence, "throughout")) {
        return TOKEN_THROUGHOUT;
    }

    if (string_equals_constant(character_sequence, "time")) {
        return TOKEN_TIME;
    }

    if (string_equals_constant(character_sequence, "timeprecision")) {
        return TOKEN_TIMEPRECISION;
    }

    if (string_equals_constant(character_sequence, "timeunit")) {
        return TOKEN_TIMEUNIT;
    }

    if (string_equals_constant(character_sequence, "tran")) {
        return TOKEN_TRAN;
    }

    if (string_equals_constant(character_sequence, "tranif0")) {
        return TOKEN_TRANIF0;
    }

    if (string_equals_constant(character_sequence, "tranif1")) {
        return TOKEN_TRANIF1;
    }

    if (string_equals_constant(character_sequence, "tri")) {
        return TOKEN_TRI;
    }

    if (string_equals_constant(character_sequence, "tri0")) {
        return TOKEN_TRI0;
    }

    if (string_equals_constant(character_sequence, "tri1")) {
        return TOKEN_TRI1;
    }

    if (string_equals_constant(character_sequence, "triand")) {
        return TOKEN_TRIAND;
    }

    if (string_equals_constant(character_sequence, "trior")) {
        return TOKEN_TRIOR;
    }

    if (string_equals_constant(character_sequence, "trireg")) {
        return TOKEN_TRIREG;
    }

    if (string_equals_constant(character_sequence, "type")) {
        return TOKEN_TYPE;
    }

    if (string_equals_constant(character_sequence, "typedef")) {
        return TOKEN_TYPEDEF;
    }

    if (string_equals_constant(character_sequence, "union")) {
        return TOKEN_UNION;
    }

    if (string_equals_constant(character_sequence, "unique")) {
        return TOKEN_UNIQUE;
    }

    if (string_equals_constant(character_sequence, "unique0")) {
        return TOKEN_UNIQUE0;
    }

    if (string_equals_constant(character_sequence, "unsigned")) {
        return TOKEN_UNSIGNED;
    }

    if (string_equals_constant(character_sequence, "until")) {
        return TOKEN_UNTIL;
    }

    if (string_equals_constant(character_sequence, "until_width")) {
        return TOKEN_UNTIL_WIDTH;
    }

    if (string_equals_constant(character_sequence, "untyped")) {
        return TOKEN_UNTYPED;
    }

    if (string_equals_constant(character_sequence, "use")) {
        return TOKEN_USE;
    }

    if (string_equals_constant(character_sequence, "uwire")) {
        return TOKEN_UWIRE;
    }

    if (string_equals_constant(character_sequence, "var")) {
        return TOKEN_VAR;
    }

    if (string_equals_constant(character_sequence, "vectored")) {
        return TOKEN_VECTORED;
    }

    if (string_equals_constant(character_sequence, "virtual")) {
        return TOKEN_VIRTUAL;
    }

    if (string_equals_constant(character_sequence, "void")) {
        return TOKEN_VOID;
    }

    if (string_equals_constant(character_sequence, "wait")) {
        return TOKEN_WAIT;
    }

    if (string_equals_constant(character_sequence, "wait_order")) {
        return TOKEN_WAIT_ORDER;
    }

    if (string_equals_constant(character_sequence, "wand")) {
        return TOKEN_WAND;
    }

    if (string_equals_constant(character_sequence, "weak")) {
        return TOKEN_WEAK;
    }

    if (string_equals_constant(character_sequence, "weak0")) {
        return TOKEN_WEAK0;
    }

    if (string_equals_constant(character_sequence, "weak1")) {
        return TOKEN_WEAK1;
    }

    if (string_equals_constant(character_sequence, "while")) {
        return TOKEN_WHILE;
    }

    if (string_equals_constant(character_sequence, "wildcard")) {
        return TOKEN_WILDCARD;
    }

    if (string_equals_constant(character_sequence, "wire")) {
        return TOKEN_WIRE;
    }

    if (string_equals_constant(character_sequence, "with")) {
        return TOKEN_WITH;
    }

    if (string_equals_constant(character_sequence, "within")) {
        return TOKEN_WITHIN;
    }

    if (string_equals_constant(character_sequence, "wor")) {
        return TOKEN_WOR;
    }

    if (string_equals_constant(character_sequence, "xnor")) {
        return TOKEN_XNOR;
    }

    if (string_equals_constant(character_sequence, "xor")){
        return TOKEN_XOR;
    }

    return TOKEN_ERROR;
}

token_t* lexer_read_keyword(lexer_t* lexer, source_code_position_t* position) {
    string_t* keyword = string_create(lexer->arena, "");
    char ch = lexer_peek_next_character(lexer);

    while (!lexer_is_whitespace(ch)) {
        keyword = string_append_character(
            lexer->arena, keyword, lexer_read_next_character(lexer));
        ch = lexer_peek_next_character(lexer);
    }

    return token_create(lexer->arena, lexer_get_keyword_type(keyword), keyword, position);
}

int lexer_has_tokens_to_lex(lexer_t* lexer) {
    return lexer_peek_next_character(lexer) != EOF;
}

token_t* lexer_create_eof_token(lexer_t* lexer) {
    string_t* eof = string_create(lexer->arena, "");
    eof = string_append_character(lexer->arena, eof, EOF);
    source_code_position_t* position = source_code_position_copy(
        lexer->arena,
        lexer->file_stream_reader->source_code_position
    );
    return token_create(lexer->arena, TOKEN_EOF, eof, position);
}

void lexer_seek_to(lexer_t* lexer, int seek_position) {
    verilog_file_stream_reader_seek_to(lexer->file_stream_reader, seek_position);
}

long lexer_get_current_seek_position(lexer_t* lexer) {
    return lexer->file_stream_reader->seek_position;
}

token_t* lexer_read_token(
    lexer_t* lexer, 
    source_code_position_t* position, 
    lexer_read_type_t read_type
) {
    switch (read_type)
    {
    case LEXER_READ_STRING:
        return lexer_read_string(lexer, position);
    case LEXER_READ_NUMBER_SEQUENCE:
        return lexer_read_number_squence(lexer, position);
    case LEXER_READ_PUNCTUATION:
        return lexer_read_punctuation(lexer, position);
    case LEXER_READ_CHARACTER_SEQUENCE:
        return lexer_read_char_sequence(lexer, position);
    case LEXER_READ_KEYWORD:
        return lexer_read_keyword(lexer, position);
    default:
        return lexer_create_error_token(lexer);
    }
}

token_t* lexer_lex(lexer_t* lexer) {
    lexer_read_whitespace(lexer);

    if (!lexer_has_tokens_to_lex(lexer)) {
        return lexer_create_eof_token(lexer);
    }

    source_code_position_t* position = source_code_position_copy(
        lexer->arena,
        lexer->file_stream_reader->source_code_position
    );
    int current_seek_position = lexer_get_current_seek_position(lexer);
    token_t* longest_match = NULL;
    int new_seek_position = current_seek_position;
    
    for (size_t operation = 0; operation < LEXER_OPERATION_COUNT; operation++) {
        token_t* matched_token = lexer_read_token(lexer, position, operation);
        int possible_seek_position = lexer_get_current_seek_position(lexer);

        if (longest_match == NULL) {
            longest_match = matched_token;
            new_seek_position = possible_seek_position;
        }

        if (matched_token->type == TOKEN_ERROR) {
            lexer_seek_to(lexer, current_seek_position);
            continue;
        }

        if (longest_match->value->length < matched_token->value->length) {
            longest_match = matched_token;
            new_seek_position = possible_seek_position;
        }

        if (longest_match->value->length == matched_token->value->length && 
            operation == LEXER_READ_KEYWORD) {
            longest_match = matched_token;
            new_seek_position = possible_seek_position;
        }

        lexer_seek_to(lexer, current_seek_position);
    }

    lexer_seek_to(lexer, new_seek_position);

    return longest_match;
}

void lexer_close(lexer_t* lexer) {
    fclose(lexer->file_stream_reader->file);
}
