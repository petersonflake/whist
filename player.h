#ifndef PLAYER_H
#define PLAYER_H
#include <string.h>
#include "whist.h"
#include "ai.h"

typedef struct _player player;

typedef struct _player {
    char *name;
    stack *hand;
    card* (*get_move)(player *self, stack* trick, suits trumps);
} player;

player* init_player(char *name);

player* init_anon_player();

player* init_human_player();

player* init_ai_player();

card* ai_make_decision(player *self, stack *trick, suits trumps);

card* human_get_decision(player *self, stack *trick, suits trumps);

#endif
