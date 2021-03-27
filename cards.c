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

void free_stack(stack *cards)
{
    if(!cards) return;
    for(int i = 0; i < cards->count; ++i) {
        if(cards->cards[i])
            free(cards->cards[i]);
    }
    free(cards);
}

card* pop_stack(stack *cards)
{
    return cards->cards[--cards->count];
}

card* get_card(card *c, stack *cards)
{
    card *found = malloc(sizeof(card));
    found->rank = c->rank;
    found->suit = c->suit;
    for(int i = 0; i < cards->count; ++i) {
        if(cards->cards[i]->rank == c->rank && cards->cards[i]->suit == c->suit) {
            int j;
            for(j = i; j < cards->count-1; ++j) {
                cards->cards[j] = cards->cards[j+1];
            }
            --cards->count;
            return found;
        }
    }
    free(found);
    found = NULL;
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
        /* Better randomness from using higher order bits,
         * which is why we are not using rand()%size.
         */
        int ind = 0 + (int)(10.0*rand()/RAND_MAX + 0.0);
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

char* print_cardf(card* c)
{
    char *buffer = malloc(1024);
    char num[20];
    strcpy(buffer, "The ");
    switch(c->rank) {
        case 12:
            strcat(buffer, "Ace of ");
            break;
        case 11:
            strcat(buffer, "King of ");
            break;
        case 10:
            strcat(buffer, "Queen of ");
            break;
        case 9:
            strcat(buffer, "Jack of ");
            break;
        default:
            sprintf(num, "%d of", c->rank+2);
            strcat(buffer, num);
            break;
    }

    switch(c->suit) {
        case HEARTS:
            strcat(buffer, "Hearts\n");
            break;
        case DIAMONDS:
            strcat(buffer, "Diamonds\n");
            break;
        case CLUBS:
            strcat(buffer, "Clubs\n");
            break;
        case SPADES:
            strcat(buffer, "Spades\n");
            break;
    }
    return buffer;
}
void print_stack(stack *s)
{
    for(int i = 0; i < s->count; ++i) {
        print_card(s->cards[i]);
    }
}

char* print_stackf(stack *s)
{
    char *buffer = malloc(4096);
    *buffer = '\0';
    for(int i = 0; i < s->count; ++i) {
        char *c = print_cardf(s->cards[i]);
        strcat(buffer, c);
        free(c);
    }
    return buffer;
}
#endif
