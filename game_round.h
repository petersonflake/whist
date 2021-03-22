#ifndef GAME_ROUND_H
#define GAME_ROUND_H
#include "player.h"
#include "ai.h"

typedef struct {
    player **players;
    stack *deck;
    directions current_turn;
    int game_id;
    int team_1_score;
    int team_2_score;
} game_round;

game_round* create_game(player *player_N, player *player_E, player *player_S, player *player_W);

int play_card(game_round *round, directions dir, card *c);
#endif
