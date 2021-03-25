#ifndef SERVER_H
#define SERVER_H
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "event_queue.h"

void *server(void *);

extern str_vector *message_queue;

#endif
