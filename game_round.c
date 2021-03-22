#include "game_round.h"

game_round* create_game(player *player_N, player *player_E, player *player_S, player *player_W)
{
    /* To identify game when multiple games are in progress */
    static int id = 0;
    game_round *new_game = malloc(sizeof(game_round));
    new_game->game_id = ++id;
    new_game->current_turn = NORTH;
    new_game->deck = create_deck();
    /* Array of players */
    new_game->players = calloc(4, sizeof(player*));
    new_game->players[NORTH] = player_N;
    new_game->players[EAST] = player_E;
    new_game->players[SOUTH] = player_S;
    new_game->players[WEST] = player_W;
    /* Shuffle Three Times */
    shuffle_cards(new_game->deck);
    shuffle_cards(new_game->deck);
    shuffle_cards(new_game->deck);

    /* Distribute 13 cards to each player */
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 13; ++j) {
            push_card(new_game->players[i]->hand, pop_stack(new_game->deck));
        }
    }
    /* Sort player hands */
    for(int i = 0; i < 4; ++i) {
        sort_cards(new_game->players[i]->hand);
    }
    return new_game;
}

int play_card(game_round *round, directions dir, card *c);
