#ifndef CARDS_H
#define CARDS_H
#include <stdlib.h>
#define DEV 1
#ifdef DEV
#include <stdio.h>
#include <string.h>
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

stack* create_hand();

stack* create_deck();

void free_stack(stack *cards);

card* pop_stack(stack *cards);

card* get_card(card *c, stack *cards);

card* find_card(card *c, stack *cards);

int push_card(stack *cards, card *new_card);

void shuffle_cards(stack *cards);

int card_compare(card *left, card *right);

void sort_cards(stack *cards);

#ifdef DEV
void print_card(card* c);

char* print_cardf(card* c);

void print_stack(stack *s);

char* print_stackf(stack *s);
#endif
#endif
