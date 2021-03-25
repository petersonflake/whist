#include "server.h"

extern str_vector *message_queue;

void *server(void *arg)
{
    int cnt = 0;
    /* Server Options */
    int listen_port = 4545; /* TODO: Set this with config */
    int server_fd, new_socket;
    struct sockaddr_in address;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(listen_port);
    int addrlen = sizeof(address);
    /* Server Process */
    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address))) {
        return NULL;
    }

    static char *buffer = NULL;
    static char *msg = NULL;
    if(!msg) msg = malloc(4096);
    if(!buffer) buffer = malloc(4096);
    listen(server_fd, 5);
    event *ne;

    while(1) {
        memset(buffer, '\0', 4096);
        memset(msg, '\0', 4096);
        new_socket = accept(server_fd,
                (struct sockaddr*)&address, (socklen_t*)&addrlen);
        read(new_socket, buffer, 4095);
        for(int i = 0; i < message_queue->count; ++i) {
            fprintf(stderr, "Message: %s\n", msg);
            strcpy(msg, message_queue->data[i]);
            send(new_socket, msg, strlen(msg)+1, 0);
        }
        strcpy(msg, "Message Received");
        fprintf(stderr, "Responding to connection\n");
        fprintf(stderr, "Received %s\n", buffer);
        enqueue_event(global_event_q, new_event(strdup(buffer)));
        send(new_socket, msg, strlen(msg)+1, 0);
        close(new_socket);
        ++cnt;
    }

    return NULL;
}
