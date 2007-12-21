/***************************************************************************
 *   This file is part of the 'utils' projects                             *
 *                                                                         *
 *   'utils' projects                                                      *
 *                                                                         *
 *   Copyright (C) 2006, 2008 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://www.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/                                                                


#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>

#include "assert.h"
#include "vector.h"

#define RECV_BUFF_SIZE 256

int server_run;


int start_server(user_pool_t * existing_users, port_t port)
{
    server_run = 1;

    srand(time(0));

    channel_pool_t * channels = create_channel_pool();
    vector_t * users = create_vector(16);

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

    dbg_printf("server started\n");

    while(server_run)
    {
        read_fds = master;
        if(select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
	{
	    if(errno == EINTR)
	    {
		dbg_printf("select interrupted\n");
		if(server_run)
		{
		    puts("\n\nAccount list:");
		    print_user_pool(existing_users, stdout);
		    puts("\n\nConnected users:");
		    print_entry_vector(users, stdout);
		    puts("\n\nChannel list:");
		    print_channel_pool(channels, stdout);
		    continue;
		}
		break;
	    }

            perror("select");
	    return EXIT_FAILURE;
        }
//parcourir ls entries, sortie le listen...
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

			vector_add_element(users,
					   create_channel_entry(newfd));

                        dbg_printf("incoming connection from %s on socket %d\n",
				   inet_ntoa(rmaddr.sin_addr), newfd);
			//send_challenge(newfd);

                    }

                }
		else
		{
		    char buf[RECV_BUFF_SIZE];
		    int nbytes = recv(i, buf, RECV_BUFF_SIZE, 0);

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
                    //process_incoming_data();
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
    

    free_channel_pool(channels);

    size_t s = vector_size(users);
    size_t u;

    for(u = 0; u < s; ++u)
    {
	channel_entry_t * e = get_entry_at(users, u);
	if(!e->channel)
	    free_channel_entry(e);
    }
    free_vector(users, 0);

    dbg_printf("server halted\n");

    return EXIT_SUCCESS;
}

void send_challenge(int fd)
{
    char buff[CHALLENGE_SIZE];

    int i = 0;
    while(i < CHALLENGE_SIZE)
	buff[i++] = 'A' + (int)((double)('Z' - 'A') * (rand() / (double)RAND_MAX));

    
    send(fd, buff, CHALLENGE_SIZE, 0);
}

bool send_data(channel_entry_t * e, char * data, size_t len)
{
    send(e->fd, data, len, 0);
    return true;
}

bool recv_data(channel_entry_t * e, char * data, size_t len)
{
    recv(e->fd, data, len, 0);
    return true;
}

void stop_server()
{
    dbg_printf("server stop requested\n");
    server_run = 0;
}
