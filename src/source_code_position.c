#include <stdlib.h>

#include "source_code_position.h"
#include "string.h"
#include "array_list.h"

source_code_position_t* source_code_position_create(arena_t* arena, string_t* file_path) {
    source_code_position_t* source_code_position = arena_allocate(arena, sizeof(source_code_position_t));

    source_code_position->column = 1;
    source_code_position->row = 1;
    source_code_position->file_path = file_path;

    return source_code_position;
}

source_code_position_t* source_code_position_copy(arena_t* arena, source_code_position_t* source_code_position) {
    source_code_position_t* copy = source_code_position_create(
        arena,
        source_code_position->file_path
    );

    copy->column = source_code_position->column;
    copy->row = source_code_position->row;

    return copy;
}

void source_code_position_move(source_code_position_t* source_code_position, char ch) {
    if (ch == '\n') {
        source_code_position->row++;
        source_code_position->column = 0;
    } else {
        source_code_position->column++;
    }
}
