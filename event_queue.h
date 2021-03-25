#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "string_vector.h"

typedef struct {
    int game_id;
    int player_id;
    str_vector *args;
} event;

typedef struct {
    event **events;     //Events themselves
    int max_size;       //Before we wrap around
    int enque_pointer;  //We add new events here
    int deque_pointer;  //We take events from here
} event_queue;

event* new_event(char *buffer);

void free_event(event *e);

event_queue* init_event_queue(int size);

int enqueue_event(event_queue *eq, event *ne);

event* peek_event(event_queue *eq);

event* dequeue_event(event_queue *eq);

void free_event_queue(event_queue *ev);

void print_event_queue(event_queue *q);

extern event_queue *global_event_q;
#endif
