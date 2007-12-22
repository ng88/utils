#ifndef CLIENT_H
#define CLIENT_H


#include "bool.h"
#include "common.h"


int connect_to_server(char * server, port_t port,
		      char * login, char * pass,
		      char * channel, option_t options);





#endif
