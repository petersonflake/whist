#include "player.h"

player* init_player(char *name)
{
    player *new_player = malloc(sizeof(player));
    new_player->name = strdup(name);
    new_player->hand = create_hand();
    return new_player;
}

player* init_anon_player()
{
    player *new_player = malloc(sizeof(player));
    new_player->name = strdup("Anonymous");
    new_player->hand = create_hand();
    return new_player;
}

card* ai_make_decision(player *self, stack *trick, suits trumps)
{
    if(trick->cards[0])
        return choose_card(self->hand, trick, trumps);
    else
        return choose_lead(self->hand, trumps);
}

card* human_get_decision(player *self, stack *trick, suits trumps)
{
    char *buffer = malloc(1024);
    size_t len;
    getline(&buffer, &len, stdin);
    buffer = strtok(buffer, "\n");
    suits suit;
    int rank;
    if(!buffer) {
        exit(0);
    }

    suit = atoi(strtok(buffer, " \t,"));
    rank = atoi(strtok(NULL, " \t,"));
    card *c = create_card(suit, rank);
    return get_card(c, self->hand);
}
