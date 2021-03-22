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
    card *strongest_card = malloc(sizeof(card));
    strongest_card = NULL;
    for(int i = 0; i < hand->count; ++i) {
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
    if(&hand->cards[hand->count-1] > &strongest_card)
        memmove(&strongest_card, &strongest_card+1, sizeof(card*));
        //memmove(&cards->cards[i], &cards->cards[i+1], sizeof(&cards->cards[i])*(cards->count-i-1));
    --hand->count;
    return strongest_card;
}

/*
 * AI strategy here is to pick the strongest card of the suit
 * led if it would beat the first card played, or the weakest
 * if it would not.  If the ai does not have any cards of the suit
 * played, it plays the weakest card of its weakest suit.
 *
 * TODO:  Make it check other cards played if it is not
 * the second player.  Also, make it take trumps into account.
 */
card* choose_card(stack *hand, trick *t, suits trumps)
{
    suits lead = t->cards[0]->suit;
    suits counts[4] = { 0 };
    int has_lead_suit = 0;
    for(int i = 0; i < hand->count; ++i) {
        if(hand->cards[i]->suit == lead) {
            ++counts[hand->cards[i]->suit];
            has_lead_suit = 1;
            break;
        }
    }
    if(has_lead_suit) {
        card *strongest_card = hand->cards[0];
        card *weakest_card = hand->cards[0];
        for(int i = 0; i < hand->count; ++i) {
            if(hand->cards[i]->suit == lead) {
                if(hand->cards[i]->rank > strongest_card->rank)
                    strongest_card = hand->cards[i];
                if(hand->cards[i]->rank < weakest_card->rank)
                    weakest_card = hand->cards[i];
            }
        }
        if(t->cards[t->count-1]->rank > strongest_card->rank) {
            if(&hand->cards[hand->count-1] > &strongest_card)
                memmove(&strongest_card, &strongest_card+1, sizeof(card*));
            --hand->count;
            return strongest_card;
        } else {
            if(&hand->cards[hand->count-1] > &weakest_card)
                memmove(&weakest_card, &weakest_card+1, sizeof(card*));
            --hand->count;
            return weakest_card;
        }

    } else {
        suits weakest_suit = counts[0];
        for(int i = 0; i < 4; ++i)
            if(counts[i] < weakest_suit)
                weakest_suit = counts[i];

        card *weakest_card = NULL;
        for(int i = 0; i < hand->count; ++i) {
            if(weakest_card) {
                if(hand->cards[i]->suit == weakest_suit)
                    if(hand->cards[i]->rank < weakest_card->rank)
                        weakest_card = hand->cards[i];
            } else {
                if(hand->cards[i]->suit == weakest_suit)
                    weakest_card = hand->cards[i];
            }
        }
        if(&hand->cards[hand->count-1] > &weakest_card)
            memmove(&weakest_card, &weakest_card+1, sizeof(card*));
        --hand->count;
        return weakest_card;
    }
}
