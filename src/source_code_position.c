#include <stdlib.h>

#include "source_code_position.h"
#include "string.h"
#include "array_list.h"

source_code_position_t* create_source_code_position(string_t* file_path) {
    source_code_position_t* source_code_position = malloc(sizeof(source_code_position_t));

    source_code_position->column = 1;
    source_code_position->row = 1;
    source_code_position->file_path = file_path;

    return source_code_position;
}

source_code_position_t* source_code_position_copy(source_code_position_t* source_code_position) {
    source_code_position_t* copy = create_source_code_position(
        source_code_position->file_path
    );

    copy->column = source_code_position->column;
    copy->row = source_code_position->row;

    return copy;
}

void update_source_code_position(source_code_position_t* source_code_position, char ch) {
    if (ch == '\n') {
        source_code_position->row++;
        source_code_position->column = 0;
    } else {
        source_code_position->column++;
    }
}

void source_code_position_deallocate(source_code_position_t* source_code_position) {
    free(source_code_position);
}
