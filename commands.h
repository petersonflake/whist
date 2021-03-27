#ifndef COMMANDS_H
#define COMMANDS_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
typedef union {
    char* (*fn_string)(char *args);
    void* (*fn_void_arg)(char *args, void *voidp);
    void (*fn_ret_void)(char *args);
} func;

typedef enum {
    NONE,
    STRING_ARG,
    VOID_RETURN,
    VOID_ARG,
} function_type;

typedef struct {
    char *text;
    function_type type;
    func fn;
} command;

typedef struct {
    int count;
    command **array;
} command_set;

command_set* init_commands(char **argv);

int connect_command(command_set *list, char *cmd, function_type type, func fn);

char* cmd_exec(char *cmd, void *voidp, command_set *list);

#endif
