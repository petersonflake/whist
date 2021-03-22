#ifndef AI_H
#define AI_H
#include <string.h>
#include "whist.h"

card* choose_lead(stack *hand, suits trumps);

card* choose_card(stack *hand, trick *t, suits trumps);

#endif
