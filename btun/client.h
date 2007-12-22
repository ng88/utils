#ifndef CLIENT_H
#define CLIENT_H


#include "bool.h"
#include "common.h"


/* warning, pass must be writable, this function clear it */
int connect_to_server(char * server, port_t port,
		      char * login, char * pass,
		      char * channel, option_t options);




int writeall(int fd, void * src, size_t s);

#endif
