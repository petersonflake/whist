#include "whist.h"

/*
 * AI strategy is to pick the highest
 * card of his strongest suit.  May incorporate
 * current trump suit later.
 *
 * TODO: Make it take trumps into account.
 */
card* choose_lead(stack *hand, suits trumps)
{
    /* Get counts of each suit */
    suits suit_counts[4] = { 0 };
    for(int i = 0; i < hand->count; ++i) {
        ++suit_counts[hand->cards[i]->suit];
    }
    /* Get strongest_suit */
    suits strongest_suit = 0;
    for(int i = 0; i < 4; ++i) {
        if(suit_counts[i] > suit_counts[strongest_suit])
            strongest_suit = i;
    }
    /* Get highest card of strongest suit */
    card *strongest_card;
    strongest_card = NULL;
    int i;
    for(i = 0; i < hand->count; ++i) {
        if(strongest_card) {
            if(hand->cards[i]->suit != strongest_suit)
                continue;
            if(hand->cards[i]->rank > strongest_card->rank)
                strongest_card = hand->cards[i];
        } else {
            if(hand->cards[i]->suit == strongest_suit)
                strongest_card = hand->cards[i];
        }
    }
    return get_card(strongest_card, hand);
}

/*
 * AI strategy here is to pick the strongest card of the suit
 * led if it would beat the first card played, or the weakest of the
 * same suit if it would not.  If the ai does not have any cards of
 * the suit played, it plays the weakest card of its weakest suit.
 */
card* choose_card(stack *hand, trick *t, suits trumps)
{
    suits lead = t->cards[0]->suit;
    suits counts[4] = { 0 };
    int has_lead_suit = 0;
    /* Counting cards of each suit for later, and also checking if
     * we have any cards of the suit lead. */
    for(int i = 0; i < hand->count; ++i) {
        ++counts[hand->cards[i]->suit];
        if(hand->cards[i]->suit == lead) {
            has_lead_suit = 1;
        }
    }
    if(has_lead_suit) {
        card *strongest_card = NULL;
        card *weakest_card = NULL;
        int i;
        /* Get the strongest and weakest cards of the suit led */
        for(i = 0; i < hand->count; ++i) {
            if(hand->cards[i]->suit == lead) {
                if(!strongest_card) strongest_card = hand->cards[i];
                else if(hand->cards[i]->rank >= strongest_card->rank)
                    strongest_card = hand->cards[i];
                if(!weakest_card) weakest_card = hand->cards[i];
                else if(hand->cards[i]->rank <= weakest_card->rank)
                    weakest_card = hand->cards[i];
            }
        }
        for(int i = 0; i < t->count; ++i) {
            /* Only play strongest card if it would win */
            if(t->cards[i]->suit == trumps || (t->cards[i]->suit == lead && t->cards[i]->rank > strongest_card->rank))
                return get_card(weakest_card, hand);
        }
        /* Only play strongest card if it would not take trick from an ally */
        switch(t->count) {
            case 1:
            case 3:
                return get_card(strongest_card, hand);
                break;
            case 2:
                return get_card(weakest_card, hand);
                break;
            default:
                fprintf(stderr, "Trick has wrong number of cards: %d\n", t->count);
                return NULL;
        }
    } else {
        suits weakest_suit = -1;
        for(int i = 0; i < 4; ++i) {
            if(counts[i] > 0) {
                if(weakest_suit == -1) {
                    weakest_suit = i;
                } else {
                    if(counts[i] < weakest_suit) {
                        weakest_suit = i;
                    }
                }
            }
        }
        card *weakest_card = NULL;
        int i;
        for(i = 0; i < hand->count; ++i) {
            if(weakest_card) {
                if(hand->cards[i]->suit == weakest_suit)
                    if(hand->cards[i]->rank < weakest_card->rank)
                        weakest_card = hand->cards[i];
            } else {
                if(hand->cards[i]->suit == weakest_suit)
                    weakest_card = hand->cards[i];
            }
        }
        if(!weakest_card) {
            printf("Led with ");
            print_card(t->cards[0]);
            print_stack(hand);
            printf("Unable to choose a card.\n");
            abort();
        }
        return get_card(weakest_card, hand);
    }
}
