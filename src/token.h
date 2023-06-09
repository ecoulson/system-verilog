#ifndef TOKEN_H
#define TOKEN_H

#include "string.h"
#include "arena.h"
#include "source_code_position.h"

typedef struct token_s {
    int type;
    string_t* value;
    source_code_position_t* position;
} token_t;

typedef enum token_type_e {
    TOKEN_UNKNOWN = -1,
    TOKEN_IDENTIFIER,
    TOKEN_LEFT_PARENTHESE,
    TOKEN_RIGHT_PARENTHESE,
    TOKEN_DOLLOR_SIGN,
    TOKEN_SEMI_COLON,
    TOKEN_PERIOD,
    TOKEN_CHARACTER_SEQUENCE,
    TOKEN_NUMBER_SEQUENCE,
    TOKEN_COMMA,
    TOKEN_ACCEPT_ON,
    TOKEN_EOF,
    TOKEN_ERROR,
    TOKEN_UNDERSCORE,
    TOKEN_DASH,
    TOKEN_COLON,
    TOKEN_ASTRIX,
    TOKEN_PLUS,
    TOKEN_SLASH,
    TOKEN_POUND,
    TOKEN_EQUAL,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_AMPERSAND,
    TOKEN_CAROT,
    TOKEN_PIPE,
    TOKEN_LEFT_ANGLE_BRACKET,
    TOKEN_RIGHT_ANGLE_BRACKET,
    TOKEN_LEFT_CURLY_BRACE,
    TOKEN_RIGHT_CURLY_BRACE,
    TOKEN_EXCLAMATION_MARK,
    TOKEN_PERCENT,
    TOKEN_ASPERAND,
    TOKEN_QUESTION_MARK,
    TOKEN_TILDE,
    TOKEN_BACKSLASH,
    TOKEN_QUOTATION_MARK,
    TOKEN_DOUBLE_QUOTATION_MARK,
    TOKEN_BACK_TICK,
    TOKEN_ALIAS,
    TOKEN_ALWAYS,
    TOKEN_ALWAYS_COMB,
    TOKEN_ALWAYS_FF,
    TOKEN_ALWAYS_LATCH,
    TOKEN_AND,
    TOKEN_ASSERT,
    TOKEN_ASSIGN,
    TOKEN_ASSUME,
    TOKEN_AUTOMATIC,
    TOKEN_BEFORE,
    TOKEN_BEGIN,
    TOKEN_BIND,
    TOKEN_BINS,
    TOKEN_BINSOF,
    TOKEN_BIT,
    TOKEN_BREAK,
    TOKEN_BUF,
    TOKEN_BUFIF0,
    TOKEN_BUFIF1,
    TOKEN_BYTE,
    TOKEN_CASE,
    TOKEN_CASEX,
    TOKEN_CASEZ,
    TOKEN_CELL,
    TOKEN_CHANDLE,
    TOKEN_CHECKER,
    TOKEN_CLASS,
    TOKEN_CLOCKING,
    TOKEN_CMOS,
    TOKEN_CONFIG,
    TOKEN_CONST,
    TOKEN_CONSTRAINT,
    TOKEN_CONTEXT,
    TOKEN_CONTINUE,
    TOKEN_COVER,
    TOKEN_COVERGROUP,
    TOKEN_COVERPOINT,
    TOKEN_CROSS,
    TOKEN_DEASSIGN,
    TOKEN_DEFAULT,
    TOKEN_DEFPARAM,
    TOKEN_DESIGN,
    TOKEN_DIST,
    TOKEN_DO,
    TOKEN_EDGE,
    TOKEN_ELSE,
    TOKEN_END,
    TOKEN_ENDCASE,
    TOKEN_ENDCHECKER,
    TOKEN_ENDCLASS,
    TOKEN_ENDCLOCKING,
    TOKEN_ENDCONFIG,
    TOKEN_ENDFUNCTION,
    TOKEN_ENDGENERATE,
    TOKEN_ENDGROUP,
    TOKEN_ENDINTERFACE,
    TOKEN_ENDMODULE,
    TOKEN_ENDPACKAGE,
    TOKEN_ENDPRIMITIVE,
    TOKEN_ENDPROGRAM,
    TOKEN_ENDPROPERTY,
    TOKEN_ENDSPECIFY,
    TOKEN_ENDSEQUENCE,
    TOKEN_ENDTABLE,
    TOKEN_ENDTASK,
    TOKEN_ENUM,
    TOKEN_EVENT,
    TOKEN_EVENTUALLY,
    TOKEN_EXPECT,
    TOKEN_EXPORT,
    TOKEN_EXTENDS,
    TOKEN_EXTERN,
    TOKEN_FINAL,
    TOKEN_FIRST_MATCH,
    TOKEN_FOR,
    TOKEN_FORCE,
    TOKEN_FOREACH,
    TOKEN_FOREVER,
    TOKEN_FORK,
    TOKEN_FORKJOIN,
    TOKEN_FUNCTION,
    TOKEN_GENERATE,
    TOKEN_GENVAR,
    TOKEN_GLOBAL,
    TOKEN_HIGHZ0,
    TOKEN_HIGHZ1,
    TOKEN_IF,
    TOKEN_IFF,
    TOKEN_IFNONE,
    TOKEN_IGNORE_BINS,
    TOKEN_ILLEGAL_BINS,
    TOKEN_IMPLEMENTS,
    TOKEN_IMPLIES,
    TOKEN_IMPORT,
    TOKEN_INCDIR,
    TOKEN_INCLUDE,
    TOKEN_INITIAL,
    TOKEN_INOUT,
    TOKEN_INPUT,
    TOKEN_INSIDE,
    TOKEN_INSTANCE,
    TOKEN_INT,
    TOKEN_INTEGER,
    TOKEN_INTERCONNECT,
    TOKEN_INTERFACE,
    TOKEN_INTERSECT,
    TOKEN_JOIN,
    TOKEN_JOIN_ANY,
    TOKEN_JOIN_NONE,
    TOKEN_LARGE,
    TOKEN_LET,
    TOKEN_LIBLIST,
    TOKEN_LIBRARY,
    TOKEN_LOCAL,
    TOKEN_LOCALPARAM,
    TOKEN_LOGIC,
    TOKEN_LONGINT,
    TOKEN_MACROMODULE,
    TOKEN_MATCHES,
    TOKEN_MEDIUM,
    TOKEN_MODPORT,
    TOKEN_MODULE,
    TOKEN_NAND,
    TOKEN_NEGEDGE,
    TOKEN_NETTYPE,
    TOKEN_NEW,
    TOKEN_NEXTTIME,
    TOKEN_NMOS,
    TOKEN_NOR,
    TOKEN_NOSHOWCANCELLED,
    TOKEN_NOT,
    TOKEN_NOTIF0,
    TOKEN_NOTIF1,
    TOKEN_NULL,
    TOKEN_OR,
    TOKEN_OUTPUT,
    TOKEN_PACKAGE,
    TOKEN_PACKED,
    TOKEN_PARAMETER,
    TOKEN_PMOS,
    TOKEN_POSEDGE,
    TOKEN_PRIMITIVE,
    TOKEN_PRIORITY,
    TOKEN_PROGRAM,
    TOKEN_PROPERTY,
    TOKEN_PROTECTED,
    TOKEN_PULL0,
    TOKEN_PULL1,
    TOKEN_PULLDOWN,
    TOKEN_PULLUP,
    TOKEN_PULSESTYLE_ONDETECT,
    TOKEN_PULSESTYLE_ONEVENT,
    TOKEN_PURE,
    TOKEN_RAND,
    TOKEN_RANDC,
    TOKEN_RANDCASE,
    TOKEN_RANDSEQUENCE,
    TOKEN_RCMOS,
    TOKEN_REAL,
    TOKEN_REALTIME,
    TOKEN_REF,
    TOKEN_REG,
    TOKEN_REJECT_ON,
    TOKEN_RELEASE,
    TOKEN_REPEAT,
    TOKEN_RESTRICT,
    TOKEN_RETURN,
    TOKEN_RNMOS,
    TOKEN_RPMOS,
    TOKEN_RTRAIN,
    TOKEN_RTRAINIF0,
    TOKEN_RTRAINIF1,
    TOKEN_S_ALWAYS,
    TOKEN_S_EVENTUALLY,
    TOKEN_S_NEXTTIME,
    TOKEN_S_UNTIL,
    TOKEN_S_UNTIL_WITH,
    TOKEN_SCALARED,
    TOKEN_SEQUENCE,
    TOKEN_SHORTINT,
    TOKEN_SHORTREAL,
    TOKEN_SHOWCANCELLED,
    TOKEN_SIGNED,
    TOKEN_SMALL,
    TOKEN_SOFT,
    TOKEN_SOLVE,
    TOKEN_SPECIFY,
    TOKEN_SPECPARAM,
    TOKEN_STATIC,
    TOKEN_STRING,
    TOKEN_STRONG0,
    TOKEN_STRONG1,
    TOKEN_STRUCT,
    TOKEN_SUPER,
    TOKEN_SUPPLY0,
    TOKEN_SUPPLY1,
    TOKEN_SYNC_ACCEPT_ON,
    TOKEN_SYNC_REJECT_ON,
    TOKEN_TABLE,
    TOKEN_TAGGED,
    TOKEN_TASK,
    TOKEN_THIS,
    TOKEN_THROUGHOUT,
    TOKEN_TIME,
    TOKEN_TIMEPRECISION,
    TOKEN_TIMEUNIT,
    TOKEN_TRAN,
    TOKEN_TRANIF0,
    TOKEN_TRANIF1,
    TOKEN_TRI,
    TOKEN_TRI0,
    TOKEN_TRI1,
    TOKEN_TRIAND,
    TOKEN_TRIOR,
    TOKEN_TRIREG,
    TOKEN_TYPE,
    TOKEN_TYPEDEF,
    TOKEN_UNION,
    TOKEN_UNIQUE,
    TOKEN_UNIQUE0,
    TOKEN_UNSIGNED,
    TOKEN_UNTIL,
    TOKEN_UNTIL_WIDTH,
    TOKEN_UNTYPED,
    TOKEN_USE,
    TOKEN_UWIRE,
    TOKEN_VAR,
    TOKEN_VECTORED,
    TOKEN_VIRTUAL,
    TOKEN_VOID,
    TOKEN_WAIT,
    TOKEN_WAIT_ORDER,
    TOKEN_WAND,
    TOKEN_WEAK,
    TOKEN_WEAK0,
    TOKEN_WEAK1,
    TOKEN_WHILE,
    TOKEN_WILDCARD,
    TOKEN_WIRE,
    TOKEN_WITH,
    TOKEN_WITHIN,
    TOKEN_WOR,
    TOKEN_XNOR,
    TOKEN_XOR
} token_type_t;

token_t* token_create(arena_t* arena, token_type_t type, string_t* value, source_code_position_t* source_code_position);

#endif
