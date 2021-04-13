#include "string_vector.h"

str_vector* create_empty_str_vector(int size)
{
    str_vector *new_vec = malloc(sizeof(str_vector));
    new_vec->count = 0;
    //Space for the NULL terminator.
    new_vec->space = size + 1;
    new_vec->data = calloc(new_vec->space, sizeof(char*));
    return new_vec;
}

str_vector* create_str_vector(int argc, char **argv)
{
    str_vector *new_vec = malloc(sizeof(str_vector));
    new_vec->count = argc;
    new_vec->space = argc + 1;
    new_vec->data = calloc(new_vec->space, sizeof(char*));
    for(int i = 0; i < argc; ++i) {
        new_vec->data[i] = strdup(argv[i]);
    }
    new_vec->data[argc] = NULL;
    return new_vec;
}

str_vector* copy_str_vector(str_vector* vec)
{
    str_vector *new_vec = create_str_vector(vec->count, vec->data);
    return new_vec;
}

str_vector* str_vector_from_string(char *string)
{
    str_vector* new_vec = create_empty_str_vector(5);
    char *duplicate = strdup(string);
    char *new_string = strtok(duplicate, " \n\t");
    if(push_str_vector(new_vec, new_string) == -1) {
        free_str_vector(new_vec);
        free(duplicate);
        return NULL;
    }
    while((new_string = strtok(NULL, " \n\t"))) {
        if(push_str_vector(new_vec, new_string) == -1) {
            free_str_vector(new_vec);
            free(duplicate);
            return NULL;
        }
    }
    free(duplicate);
    return new_vec;
}

str_vector* str_vector_from_string_nl(char *string)
{
    str_vector* new_vec = create_empty_str_vector(5);
    char *duplicate = strdup(string);
    char *new_string = strtok(duplicate, "\n");
    if(push_str_vector(new_vec, new_string) == -1) {
        free_str_vector(new_vec);
        free(duplicate);
        return NULL;
    }
    while((new_string = strtok(NULL, "\n"))) {
        if(push_str_vector(new_vec, new_string) == -1) {
            free_str_vector(new_vec);
            free(duplicate);
            return NULL;
        }
    }
    free(duplicate);
    return new_vec;
}

void free_str_vector(str_vector *vec)
{
    for(int i = 0; i < vec->count; ++i) {
        if(vec->data[i]) free(vec->data[i]);
    }
    free(vec->data);
    free(vec);
}

int push_str_vector(str_vector *dest, char *string)
{
    if(dest->count >= dest->space - 1) {
        dest->space += 10;
        void *tmp = realloc(dest->data, dest->space * sizeof(char*));
        if(tmp) dest->data = tmp;
        else return -1;
    }
    dest->data[dest->count++] = strdup(string);
    return dest->count;
}

int va_push_str_vector(str_vector *dest, ...)
{
    va_list valist;
    va_start(valist, dest);
    char *new_string;
    while((new_string = va_arg(valist, char*))) {
        if(push_str_vector(dest, new_string) == -1)
            return -1;
    }
    va_end(valist);
    return dest->count;
}

char* peek_str_vector(str_vector *vec)
{
    return vec->data[vec->count-1];
}

char* pop_str_vector(str_vector *vec)
{
    char *tmp = strdup(vec->data[--vec->count]);
    free(vec->data[vec->count]);
    return tmp;
}
