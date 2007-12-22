#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>

#include "client.h"
#include "assert.h"


int connect_to_server(char * server, port_t port,
		      char * login, char * pass,
		      char * channel, bool master)
{

    int sockfd;
    struct sockaddr_in dest_addr;

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
	perror("socket");
	return EXIT_FAILURE;
    }

    struct hostent * h;

    if((h = gethostbyname(server)) == NULL)
    {
        herror("gethostbyname");
        return EXIT_FAILURE;
    }

    dbg_printf("trying to connect to %s (%s:%u)...\n", server, 
	       inet_ntoa(*((struct in_addr *)h->h_addr)), port);


    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr = *((struct in_addr *)h->h_addr);
    memset(dest_addr.sin_zero, 0, sizeof(dest_addr.sin_zero));


    if(connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) == -1)
    {
	perror("connect");
	return EXIT_FAILURE;
    }

    send(sockfd, login, strlen(login), 0);


    char ch[CHALLENGE_SIZE];
    recv(sockfd, ch, CHALLENGE_SIZE, 0);


    return EXIT_SUCCESS;
}
