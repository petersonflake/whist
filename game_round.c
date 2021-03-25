#include "game_round.h"

game_round* create_game(player *player_N, player *player_E, player *player_S, player *player_W)
{
    /* To identify game when multiple games are in progress */
    static int id = 0;
    game_round *new_game = malloc(sizeof(game_round));
    new_game->game_id = id++;
    new_game->team_1_score = 0;
    new_game->team_2_score = 0;
    new_game->current_turn = rand()%4;
    new_game->trumps = rand()%4;
    /* Game function */
    new_game->play_game = play_game;
    /* Current trick being played */
    new_game->trick = malloc(sizeof(stack));
    new_game->trick->cards = calloc(4, sizeof(card*));
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
    fprintf(stderr, "Started new game: %d\n", new_game->game_id);
    return new_game;
}

game_array* init_game_array(int max_size)
{
    game_array *all_games = malloc(sizeof(game_array));
    all_games->all_games = calloc(max_size, sizeof(game_round*));
    all_games->count = 0;
    all_games->size = max_size;
    return all_games;
}

int game_array_new(game_array *ga, int options)
{
    if(ga->count >= ga->size) return -1;
    ga->all_games[ga->count] = 
        create_game(init_human_player(), init_ai_player(), init_ai_player(), init_ai_player());
    return ga->all_games[ga->count++]->game_id;
}

char *get_game_info(game_round *g)
{
    char *msg = malloc(4096);
    return msg;
}

int play_card(game_round *round, directions dir, card *c);

void *play_game(void *arg)
{
    game_round *self = (game_round*)arg;
    for(int i = 0; i < 13; ++i) {
        self->trick->count = 0;
        while(self->trick->count < 4) {
            self->current_turn = ++self->current_turn%4;
            card *chosen = NULL;
            while(!chosen) {
                chosen=self->players[self->current_turn]->get_move(self->players[self->current_turn], self->trick, self->trumps);
            }
            if(is_legal_move(self->players[self->current_turn]->hand, chosen, self->trick))
                push_card(self->trick, chosen);
            else {
                printf("Attempted illegal move.\n");
                return NULL;
            }
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
        self->current_turn = winner;
        for(int i = 0; i < 4; ++i) {
            free(self->trick->cards[i]);
            self->trick->cards[i] = NULL;
        }
        self->trick->count = 0;
    }
    printf("Winner %d\n", (self->team_2_score - self->team_1_score > 0) ? 2 : 1);
    return NULL;
}

void free_game_round(game_round *game)
{
    for(int i = 0; i < 4; ++i) {
        free_player(game->players[i]);
    }
    free(game->players);
    for(int i = 0; i < 4; ++i) {
        if(game->trick->cards[i])
            free(game->trick->cards[i]);
    }
    free(game->trick->cards);
    free(game->trick);
    for(int i = 0; i < 52; ++i)
        free(game->deck->cards[i]);
    free(game->deck->cards);
    free(game->deck);
    free(game);
}
