#ifndef PLAYER_H
#define PLAYER_H
#include <string.h>
#include <unistd.h>
#include "string_vector.h"
#include "whist.h"
#include "ai.h"
#include "line_edit.h"
#include "parse.h"

typedef struct _player player;

typedef struct _player {
    int id;
    char *name;
    stack *hand;
    card* (*get_move)(player *self, stack* trick, suits trumps);
    char *next_move;
} player;

player* init_player(char *name);

player* init_anon_player();

player* init_human_player();

player* init_ai_player();

void free_player(player *p);

card* ai_make_decision(player *self, stack *trick, suits trumps);

card* human_get_decision(player *self, stack *trick, suits trumps);

#endif
