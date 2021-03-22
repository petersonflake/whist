#include "whist.h"

directions trick_winner(trick *t, directions lead, suits trumps)
{
    if(t->count > 4) {
        fprintf(stderr, "Trick as more than four cards.\n");
        print_stack(t);
        exit(1);
    }

    card *led_card = t->cards[0];
    int index_best = 0;
    for(int i = 1; i < 4; ++i) {
        if(t->cards[i]->suit == trumps) {
            if(t->cards[index_best]->suit != trumps) {
                index_best = i;
            } else {
                if(t->cards[i]->rank > t->cards[index_best]->rank) {
                    index_best = i;
                }
            }
        } else if(t->cards[i]->suit == led_card->suit) {
            if(t->cards[index_best]->suit != trumps) {
                if(t->cards[index_best]->suit != led_card->suit) {
                    index_best = i;
                } else {
                    if(t->cards[i]->rank > t->cards[index_best]->rank) {
                        index_best = i;
                    }
                }
            }
        } else {
            /* If not playing trumps or lead suit, cannot win. */
            continue;
        }
    }
    return (index_best+lead)%4;
}

int is_legal_move(stack *hand, card *c, stack *trick)
{
    if(trick->count == 0) return 1;
    suits lead = trick->cards[0]->suit;

    int has_lead = 0;
    for(int i = 0; i < hand->count; ++i) {
        if(hand->cards[i]->suit == lead) {
            has_lead = 1;
            break;
        }
    }
    if(has_lead && c->suit == lead)
        return 1;
    else if(has_lead && c->suit != lead)
        return 0;
    else return 1;
}
