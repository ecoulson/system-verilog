#ifndef SOURCE_CODE_POSITION_H
#define SOURCE_CODE_POSITION_H
#include <stdio.h>

#include "string.h"

typedef struct source_code_position_s {
    int row;
    int column;
} source_code_position_t;

source_code_position_t* create_source_code_position(); 
void update_source_code_position(source_code_position_t* source_code_position, char ch);
void source_code_position_deallocate(source_code_position_t* source_code_position); 

#endif
