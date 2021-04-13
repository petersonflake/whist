#ifndef STRING_VECTOR_H
#define STRING_VECTOR_H
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "reallocarray.h"

typedef struct {
    char **data;
    int space;
    int count;
} str_vector;

str_vector* create_empty_str_vector(int size);

str_vector* create_str_vector(int argc, char **argv);

str_vector* copy_str_vector(str_vector *vec);

str_vector* str_vector_from_string(char *string);

str_vector* str_vector_from_string_nl(char *string);

void free_str_vector(str_vector *vec);

int push_str_vector(str_vector *dest, char *string);

int va_push_str_vector(str_vector *dest, ...);

char* peek_str_vector(str_vector *vec);

char* pop_str_vector(str_vector *vec);

#endif
