#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <netinet/in.h>
typedef uint16_t port_t;

#define SERVER_DEFAULT_PORT 12358

#define USER_MAX_PASS_SIZE 64
#define USER_MAX_LOGIN_SIZE 8
#define USER_MAX_CHANNEL_SIZE 8

#define CHALLENGE_SIZE 8

#endif
