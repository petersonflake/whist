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
    static int new_id = 0;
    player* new_human = init_anon_player();
    new_human->get_move = human_get_decision;
    new_human->next_move = NULL;
    new_human->id = new_id++;
    return new_human;
}

player* init_ai_player()
{
    player *new_ai = init_anon_player();
    new_ai->get_move = ai_make_decision;
    return new_ai;
}

void free_player(player *p)
{
    if(!p) return;
    /*
     * We do not free the individual cards because they are freed
     * when we free the deck.  The players each receive pointers allocated
     * when the deck is created, so they are de-allocated when the deck is
     * freed.  This is where the double-frees were coming from.
     */
    if(p->name) free(p->name);
    free(p->hand->cards);
    free(p->hand);
    free(p);
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
    while(!ret) {
        printf("Current hand:\n");
        print_stack(self->hand);
        printf("Current Trick\n");
        print_stack(trick);
        printf("Trumps: %d\n", trumps);
        int rank;
        suits suit;
        char *buffer = malloc(4096);
        get_line_compl("Whist>", buffer);
        parse_move(buffer, &rank, &suit);
        if(!strcmp(buffer, "quit")) {
            exit(0);
        }
        c = create_card(suit, rank);
        print_card(c);
        if(is_legal_move(self->hand, c, trick)) {
            ret = get_card(c, self->hand);
        } else {
            free(c);
            c = NULL;
            ret = NULL;
            continue;
        }
        free(c);
        free(buffer);
    }
    return ret;
}
