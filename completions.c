#include "completions.h"

comp_set *global_comps = NULL;

int comp_comp(const void *left, const void *right)
{
    const char *ll = *(const char **)left;
    const char *rr = *(const char **)right;
    return strcmp(rr, ll);
}

comp_set* init_compset(char **comps)
{
    if(!comps) return NULL;
    comp_set *tmp = malloc(sizeof(comp_set));
    int c;
    for(c = 0; comps[c]; ++c) ;
    qsort(comps, c, sizeof(char *), comp_comp);
    tmp->allocated = tmp->count = c;
    tmp->completions = malloc(sizeof(completion*) * c);
    for(int i = 0; i < c; ++i) {
        tmp->completions[i] = malloc(sizeof(completion));
    }
    for(int i = 0; i < tmp->count; ++i) {
        tmp->completions[i]->text = strdup(comps[i]);
        tmp->completions[i]->length = strlen(tmp->completions[i]->text);
    }
    return tmp;
}

char* get_comp(char *text, comp_set *comps)
{
    int tlen = strlen(text);
    for(int i = 0; i < comps->count; ++i) {
        if(comps->completions[i]->length >= tlen) {
            if(!strncmp(text, comps->completions[i]->text, tlen))
                return comps->completions[i]->text;
        }
    }
    return NULL;
}

char* complete_text(char *text, comp_set *comps)
{
    if(!text) return NULL;
    int tlen = strlen(text);
    if(tlen < 1) return NULL;
    for(int i = 0; i < comps->count; ++i) {
        if(comps->completions[i]->length >= tlen) {
            if(!strncmp(comps->completions[i]->text, text, tlen))
                return &comps->completions[i]->text[tlen];
        }
    }
    return NULL;
}

void print_completions(comp_set *comps)
{
    for(int i = 0; i < comps->count; ++i) {
        printf("%s\n", comps->completions[i]->text);
    }
}

void free_compset(comp_set *comps)
{
    if(!comps) return;
    for(int i = 0; i < comps->count; ++i) {
        free(comps->completions[i]);
    }
    free(comps->completions);
    free(comps);
}
