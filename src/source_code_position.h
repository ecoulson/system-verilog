#ifndef SOURCE_CODE_POSITION_H
#define SOURCE_CODE_POSITION_H
#include <stdio.h>

#include "string.h"
#include "arena.h"

typedef struct source_code_position_s {
    int row;
    int column;
    string_t* file_path;
} source_code_position_t;

source_code_position_t* create_source_code_position(arena_t* arena, string_t* file_path); 
source_code_position_t* source_code_position_copy(arena_t* arena, source_code_position_t* source_code_position);
void update_source_code_position(source_code_position_t* source_code_position, char ch);

#endif
