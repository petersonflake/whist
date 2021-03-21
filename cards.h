#ifndef CARDS_H
#define CARDS_H
#include <stdlib.h>
#define DEV 1
#ifdef DEV
#include <stdio.h>
#endif

typedef enum {
    HEARTS,
    DIAMONDS,
    SPADES,
    CLUBS
} suits;

typedef struct {
    suits suit;
    int rank;
} card;

typedef struct {
    int count;
    card **cards;
} stack;

card* create_card(suits suit, int rank);

stack* create_deck();

card* pop_stack(stack *cards);

card* get_card(card *c, stack *cards);

card* find_card(card *c, stack *cards);

int push_card(stack *cards, card *new_card);

void shuffle_cards(stack *cards);

int card_compare(card *left, card *right);

void sort_cards(stack *cards);

#ifdef DEV
void print_card(card* c);

void print_stack(stack *s);
#endif
#endif
