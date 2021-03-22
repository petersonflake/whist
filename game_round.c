#include "game_round.h"

game_round* create_game(player *player_N, player *player_E, player *player_S, player *player_W)
{
    /* To identify game when multiple games are in progress */
    static int id = 0;
    game_round *new_game = malloc(sizeof(game_round));
    new_game->game_id = ++id;
    new_game->current_turn = NORTH;
    new_game->trumps = rand()%4;
    /* Game function */
    new_game->play_game = play_game;
    /* Current trick being played */
    new_game->trick = malloc(sizeof(stack));
    /* Create deck */
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

void play_game(game_round *self)
{
    for(int i = 0; i < 13; ++i) {
        //self->trick = malloc(sizeof(stack));
        self->trick->cards = calloc(4, sizeof(card*));
        self->trick->count = 0;
        for(int j = 0; j < 4; ++j) {
            card *played_card = self->players[(j+self->current_turn)%4]->get_move(self->players[(self->current_turn+j)%4],
                    self->trick, self->trumps);
            if(is_legal_move(self->players[(self->current_turn+j)%4]->hand, played_card, self->trick))
                push_card(self->trick, played_card);
        }
        directions winner;
        switch((winner = trick_winner(self->trick, self->current_turn, self->trumps))) {
            case NORTH:
            case SOUTH:
                ++self->team_1_score;
                break;
            case EAST:
            case WEST:
                ++self->team_2_score;
                break;
        }
        printf("player %d won trick %d\n", winner, i+1);
        self->current_turn = winner;
        //for(int i = 0; i < 4; ++i) {
        //    free(self->trick->cards[i]);
        //}
    }
    printf("Team 1: %d\nTeam 2: %d\n", self->team_1_score, self->team_2_score);
}
