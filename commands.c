#include "commands.h"

char* get_args(char *string)
{
    char *in = string;
    while(*in && *in != ' ') {
        ++in;
    }
    return ++in;
}

char* get_cmd(char *string)
{
    static char cmd[100];
    memset(cmd, '\0', 100);             /* Ensure previous command cannot be returned twice. */
    char *in = string;
    for(; *in && *in == ' '; ++in);     /* Skip any leading spaces */
    char *out = cmd;
    for(; *in && *in != ' '; ++in) {    /* Copy text up to first space */
        *out++ = *in;
    }
    *out++ = '\0';                      /* Null terminate the string */
    return cmd;
}

static int comp_c_len(const void *left, const void *right)
{
    const char *ll = *(const char **)left;
    const char *rr = *(const char **)right;
    return strcmp(rr, ll);
}

command_set* init_commands(char **argv)
{
    if(!argv) return NULL;
    int i;
    for(i = 0; argv[i]; ++i);
    if(!i) return NULL;
    qsort(argv, i, sizeof(char*), comp_c_len);
    command_set *tmp = malloc(sizeof(command_set));
    tmp->array = malloc(sizeof(command*)*i);
    tmp->count = i;
    for(int i = 0; i < tmp->count; ++i) {
        tmp->array[i] = malloc(sizeof(command));
        tmp->array[i]->fn.fn_string = NULL;
        tmp->array[i]->type = NONE;
        tmp->array[i]->text = strdup(argv[i]);
    }
    return tmp;
}

int connect_command(command_set *list, char *cmd, function_type type, func fn)
{
    int res = 1;
    int i = 0;
    for(i = 0; i < list->count; ++i) {
        if(!strcmp(list->array[i]->text, cmd)) {
            switch(type) {
                case STRING_ARG:
                    list->array[i]->fn.fn_string = fn.fn_string;
                    list->array[i]->type = type;
                    res = 0;
                    break;
                case VOID_RETURN:
                    list->array[i]->fn.fn_ret_void = fn.fn_ret_void;
                    list->array[i]->type = type;
                    res = 0;
                    break;
                case VOID_ARG:
                    list->array[i]->fn.fn_void_arg = fn.fn_void_arg;
                    list->array[i]->type = type;
                    res = 0;
                    break;
                default:
                    return 1;
            }
        }
    }
    return res;
}

char* cmd_exec(char *cmd, void *voidp, command_set *list)
{
    char *args = get_args(cmd);
    char *cmd_string = get_cmd(cmd);
    for(int i = 0; i < list->count; ++i) {
        if(!strcmp(list->array[i]->text, cmd_string)) {
            switch(list->array[i]->type) {
                case NONE:
                    printf("Command not implemented: %s\n", cmd_string);
                    return NULL;
                    break;
                case STRING_ARG:
                    return list->array[i]->fn.fn_string(args);
                    break;
                case VOID_RETURN:
                    list->array[i]->fn.fn_ret_void(args);
                    return NULL;
                    break;
                case VOID_ARG:
                    printf("Command not implemented: %s\n", args);
                    return list->array[i]->fn.fn_void_arg(args, voidp);
                default:
                    return NULL;
            }
        }
    }
    printf("Command not found: %s\n", cmd);
    return NULL;
}
