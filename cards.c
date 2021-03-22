#include "cards.h"

card* create_card(suits suit, int rank)
{
    card* new_card = malloc(sizeof(card));
    new_card->suit = suit;
    new_card->rank = rank;
    return new_card;
}

stack* create_hand()
{
    stack *new_hand = malloc(sizeof(stack));
    new_hand->count = 0;
    new_hand->cards = calloc(13, sizeof(card*));
    return new_hand;
}

stack* create_deck()
{
    stack *new_deck = malloc(sizeof(stack));
    new_deck->count = 52;
    new_deck->cards = calloc(52, sizeof(card*));
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 13; ++j) {
            new_deck->cards[i*13+j] = create_card(i, j);
        }
    }
    return new_deck;
}

card* pop_stack(stack *cards)
{
    return cards->cards[--cards->count];
}

card* get_card(card *c, stack *cards)
{
    for(int i = 0; i < cards->count; ++i) {
        if(cards->cards[i]->rank == c->rank
                && cards->cards[i]->suit == c->suit) {
            if(&cards->cards[i] < &cards->cards[cards->count-1])
                memmove(&cards->cards[i], &cards->cards[i+1], sizeof(&cards->cards[i])*(cards->count-i-1));
            --cards->count;
            return c;
        }
    }
    return NULL;
}

card* find_card(card *c, stack *cards);

int push_card(stack *cards, card *new_card)
{
    int res = 0;
    cards->cards[cards->count++] = new_card;
    return res;
}

void shuffle_cards(stack *cards)
{
    for(int i = 0; i < cards->count; ++i) {
        card *tmp = cards->cards[i];
        int ind = rand()%cards->count;
        cards->cards[i] = cards->cards[ind];
        cards->cards[ind] = tmp;
    }
}

int card_compare(card *left, card *right)
{
    if(left->rank == right->rank && left->suit == right->suit)
        return 0;
    if(left->suit < right->suit)
        return -1;
    else if(left->suit > right->suit)
        return 1;
    else if(left->suit == right->suit) {
        return (left->rank > right->rank);
    } else {
        fprintf(stderr, "This should not be reachable\n");
        exit(1);
    }
}

static int qsort_cards(const void *left, const void *right)
{
    return card_compare((card*)left, (card*)right);
}

void sort_cards(stack *cards)
{
    qsort(cards->cards, cards->count, sizeof(card*), qsort_cards);
}

#ifdef DEV
void print_card(card* c)
{
    printf("The ");
    switch(c->rank) {
        case 12:
            printf("Ace of ");
            break;
        case 11:
            printf("King of ");
            break;
        case 10:
            printf("Queen of ");
            break;
        case 9:
            printf("Jack of ");
            break;
        default:
            printf("%d of ", c->rank + 2);
            break;
    }

    switch(c->suit) {
        case HEARTS:
            printf("Hearts\n");
            break;
        case DIAMONDS:
            printf("Diamonds\n");
            break;
        case CLUBS:
            printf("Clubs\n");
            break;
        case SPADES:
            printf("Spades\n");
            break;
    }
}
void print_stack(stack *s)
{
    for(int i = 0; i < s->count; ++i) {
        print_card(s->cards[i]);
    }
}
#endif
