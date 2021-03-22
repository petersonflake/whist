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

player* init_human_player()
{
    player* new_human = init_anon_player();
    new_human->get_move = human_get_decision;
    return new_human;
}

player* init_ai_player()
{
    player *new_ai = init_anon_player();
    new_ai->get_move = ai_make_decision;
    return new_ai;
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
    card *c = NULL;
    card *ret = NULL;
    char *buffer = malloc(1024);
    size_t len;
    suits suit;
    int rank;
    while(!ret) {
        printf("Your hand:\n");
        print_stack(self->hand);
        printf("Current trick:\n");
        printf("Trumps: %d\n", trumps);
        print_stack(trick);
        getline(&buffer, &len, stdin);
        buffer = strtok(buffer, "\n");
        if(!buffer) {
            exit(0);
        }
        suit = atoi(strtok(buffer, " \t,"));
        rank = atoi(strtok(NULL, " \t,"));
        c = create_card(suit, rank);
        ret = get_card(c, self->hand);
    }
    return ret;
}
