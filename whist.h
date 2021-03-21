#ifndef WHIST_H
#define WHIST_H
#include "cards.h"

typedef stack trick;

typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
} directions;

/* Return the direction that won the trick, based
 * on the direction that lead, and current trumps.
 */
directions trick_winner(trick *t, directions lead, suits trumps);

#endif
