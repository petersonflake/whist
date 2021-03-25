#ifndef GAME_ROUND_H
#define GAME_ROUND_H
#include "player.h"
#include "ai.h"

typedef struct _gameround game_round;

typedef struct _gameround{
    player **players;
    stack *deck;
    stack *trick;
    suits trumps;
    void *(*play_game)(void *arg);
    directions current_turn;
    int game_id;
    int team_1_score;
    int team_2_score;
} game_round;

typedef struct {
    game_round **all_games;
    int size;
    int count;
} game_array;

game_round* create_game(player *player_N, player *player_E, player *player_S, player *player_W);

game_array* init_game_array(int max_size);

int game_array_new(game_array *ga, int options);

char *get_game_info(game_round *g);

int play_card(game_round *round, directions dir, card *c);

void *play_game(void *arg);

void free_game_round(game_round *game);
#endif
