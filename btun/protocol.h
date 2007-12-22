#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <netinet/in.h>
typedef uint16_t port_t;

#define SERVER_DEFAULT_PORT 12358

#define USER_MAX_PASS_SIZE 64
#define USER_MAX_LOGIN_SIZE 8
#define USER_MAX_CHANNEL_SIZE 16

#define CHALLENGE_SIZE 8


/* channel options */
enum
{
    OPT_MASTER = 1
};

typedef unsigned char option_t;


/* channel access  */
enum
{
    CA_GRANTED = 0,
    CA_DENIED = 1,
    CA_ALREADY_EXISTS = 2,
};


#endif
