#ifndef COMPLETIONS_H
#define COMPLETIONS_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char *text;
    int length;
} completion;

typedef struct {
    int count;
    int allocated;
    completion **completions;
} comp_set;

comp_set* init_compset(char **comps);

char* get_comp(char *text, comp_set *comps);

char* complete_text(char *text, comp_set *comps);

void print_completions(comp_set *comps);

void free_compset(comp_set *comps);

extern comp_set *global_comps;

#endif
