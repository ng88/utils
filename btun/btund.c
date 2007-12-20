#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "assert.h"
#include "channel.h"

#define SERVER_BACKLOG 5
#define DEFAULT_PORT 12358

typedef uint16_t port_t;



int start_server(port_t port)
{
    fd_set master;
    fd_set read_fds;

    struct sockaddr_in myaddr;
    struct sockaddr_in rmaddr;

    int fdmax;
    int fdlisten;

    int yes = 1;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    if((fdlisten = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    if(setsockopt(fdlisten, SOL_SOCKET, SO_REUSEADDR,
		  &yes, sizeof(yes)) == -1)
    {
        perror("setsockopt");
        return EXIT_FAILURE;
    }


    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(port);
    memset(myaddr.sin_zero, 0, sizeof(myaddr.sin_zero));

    if(bind(fdlisten, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1)
    {
        perror("bind");
        return EXIT_FAILURE;
    }


    if(listen(fdlisten, SERVER_BACKLOG) == -1)
    {
        perror("listen");
	return EXIT_FAILURE;
    }

    FD_SET(fdlisten, &master);

    fdmax = fdlisten;

    while(1)
    {
        read_fds = master;
        if(select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
	{
            perror("select");
	    return EXIT_FAILURE;
        }

	int i;
        for(i = 0; i <= fdmax; i++)
	{
            if(FD_ISSET(i, &read_fds))
	    {
                if(i == fdlisten)
		{
                    socklen_t addrlen = sizeof(rmaddr);

		    int newfd = accept(fdlisten,
				       (struct sockaddr *)&rmaddr,
                                       &addrlen);

                    if(newfd == -1)
		    {
                        perror("accept");
                    }
		    else
		    {
                        FD_SET(newfd, &master);

                        if (newfd > fdmax)
                            fdmax = newfd;

                        dbg_printf("incoming connection from %s on socket %d\n",
				   inet_ntoa(rmaddr.sin_addr), newfd);
                    }

                }
		else
		{
		    char buf[256];
		    int nbytes = recv(i, buf, sizeof(buf), 0);

		    if(nbytes <= 0)
		    {
			if(nbytes <= 0)
			{
			    dbg_printf("connection to socket %d closed\n", i);
			}
			else
			    perror("recv");

                        close(i);
                        FD_CLR(i, &master);
		    }
		    else
		    {

			dbg_printf("data received from socket %d\n", i);

			int j;

                        for(j = 0; j <= fdmax; j++)
			{
                            if(FD_ISSET(j, &master))
			    {
                                if (j != fdlisten && j != i)
				{

                                    if (send(j, buf, nbytes, 0) == -1)
                                        perror("send");

                                }
                            }
                        }
                    }
                }
            }
        }
    }
    

    return EXIT_SUCCESS;
}


int main(int argc, char ** argv)
{

    int r = start_server(DEFAULT_PORT);

    return r;
}