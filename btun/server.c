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

/* TODO
   option -d (lancement en demon)
   option pty
   option master (celui qui demarre en master envoir a tout le monde)
 */

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

    size_t s;
    size_t i;

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


	if(FD_ISSET(fdlisten, &read_fds))
	{
	    socklen_t addrlen = sizeof(rmaddr);
	    
	    int fd = accept(fdlisten,
			       (struct sockaddr *)&rmaddr,
			       &addrlen);
	    
	    if(fd == -1)
	    {
		perror("accept");
	    }
	    else
	    {
		
		FD_SET(fd, &master);
		
		if (fd > fdmax)
		    fdmax = fd;
		
		vector_add_element(users,
				   create_channel_entry(fd));
		
		dbg_printf("incoming connection from %s on socket %d\n",
			   inet_ntoa(rmaddr.sin_addr), fd);
		//send_challenge(fd);
		
	    }
	    
	}

	s = vector_size(users);

        for(i = 0; i < s; i++)
	{
	    channel_entry_t * e = get_entry_at(users, i);

            if(FD_ISSET(e->fd, &read_fds))
	    {

		char buf[RECV_BUFF_SIZE];
		int n = recv(e->fd, buf, RECV_BUFF_SIZE, 0);

		if(n <= 0)
		{
		    if(n <= 0)
		    {
			dbg_printf("connection to socket %d closed\n", e->fd);
		    }
		    else
		    {
			perror("recv");
			dbg_printf("recv error on socket %d, closing\n", e->fd);
		    }

		    close(e->fd);
		    FD_CLR(e->fd, &master);


		    vector_del_element_at(users, i);
		    i--; s--;

		    /* si on est le max il faut trouver le nouveau max */
		    if(e->fd == fdmax)
			get_highest_fd(users, fdlisten);

		    free_channel_entry(e);

		}
		else
                    process_incoming_data(buf, n, existing_users,
					  users, e, &master);
            }
        }

    }
    

    free_channel_pool(channels);

    s = vector_size(users);

    for(i = 0; i < s; ++i)
    {
	channel_entry_t * e = get_entry_at(users, i);
	if(!e->channel)
	    free_channel_entry(e);
    }
    free_vector(users, 0);

    dbg_printf("server halted\n");

    return EXIT_SUCCESS;
}

void process_incoming_data(char * buf, int n, user_pool_t * existing_users, 
			   vector_t * u, channel_entry_t * e, fd_set * fs)
{

    c_assert(e);

    dbg_printf("data received from socket %d\n", e->fd);

    size_t s = vector_size(u);
    size_t i;

    for(i = 0; i < s; i++)
    {
	channel_entry_t * ce = get_entry_at(u, i);

	if(ce != e && FD_ISSET(ce->fd, fs))
	{
	    if (send(ce->fd, buf, n, MSG_NOSIGNAL) == -1)
		perror("send");

	}
    }
}

int get_highest_fd(vector_t * u, int fdlist)
{
    size_t s = vector_size(u);
    size_t i;

    int r = fdlist;

    for(i = 0; i < s; i++)
    {
	channel_entry_t * e = get_entry_at(u, i);
	if(e->fd > r)
	    r = e->fd;
    }

    return r;
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
    server_run = 0;
}
