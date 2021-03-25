#include "event_queue.h"

event_queue *global_event_q;

event* new_event(char *buffer)
{
    event *ev = malloc(sizeof(event));
    ev->args = copy_str_vector(str_vector_from_string(buffer));
    for(char *in = ev->args->data[0]; *in; ++in) {
        if(!isdigit(*in)) {
            fprintf(stderr, "Attempting to add event without first ID\n");
            free_str_vector(ev->args);
            free(ev);
            return NULL;
        }
    }
    for(char *in = ev->args->data[1]; *in; ++in) {
        if(!isdigit(*in)) {
            fprintf(stderr, "Attempting to add event without second ID\n");
            free_str_vector(ev->args);
            free(ev);
            return NULL;
        }
    }
    ev->game_id = atoi(ev->args->data[0]);
    ev->player_id = atoi(ev->args->data[1]);
    return ev;
}

void free_event(event *e)
{
    if(!e) return;
    free_str_vector(e->args);
    free(e);
    e = NULL;
}

event_queue* init_event_queue(int size)
{
    event_queue *evq = malloc(sizeof(event_queue));
    evq->events = calloc(size, sizeof(event*));
    evq->max_size = size;
    evq->deque_pointer = 0;
    evq->enque_pointer = 0;
    return evq;
}

int enqueue_event(event_queue *eq, event *ne)
{
    if(eq->events[eq->enque_pointer]) {
        free_event(eq->events[eq->enque_pointer]);
        eq->events[eq->enque_pointer] = NULL;
    }
    eq->events[eq->enque_pointer] = ne;
    eq->enque_pointer = ++eq->enque_pointer%eq->max_size;
    return 0;
}

event* peek_event(event_queue *eq)
{
    /* Should be NULL if there is nothing in the queue */
    return eq->events[eq->deque_pointer];
}

event* dequeue_event(event_queue *eq)
{
    if(eq->deque_pointer == eq->enque_pointer) return NULL;
    /* Whoever requests the event must free it. */
    if(eq->events[eq->deque_pointer]) {
        return eq->events[eq->deque_pointer++];
    } else {
        return NULL;
    }
}
void free_event_queue(event_queue *ev)
{
    if(!ev) return;
    while(ev->deque_pointer != ev->enque_pointer) {
        if(ev->events[ev->deque_pointer])
            free_event(ev->events[ev->deque_pointer]);
        ev->deque_pointer = ++ev->deque_pointer%ev->max_size;
    }
    free(ev->events);
    free(ev);
}

void print_event_queue(event_queue *q)
{
    int i = q->deque_pointer;
    for(;i < q->enque_pointer; ++i) {
        fprintf(stdout, "Event with game id: %d"
                " and player id %d\n",
                q->events[i]->game_id, q->events[i]->player_id);
        str_vector *cur = q->events[i]->args;
        for(int j = 0; j < cur->count; ++j) {
            fprintf(stdout, "%s\n", cur->data[j]);
        }
    }
}
