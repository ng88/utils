#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#define SERVER_BACKLOG 5
#define SERVER_DEFAULT_PORT 12358

#define SERVER_MAX_CHANNEL 10
#define SERVER_MAX_USER_PER_CHANNEL 10

typedef uint16_t port_t;


int start_server(port_t port);

void stop_server();

#endif
