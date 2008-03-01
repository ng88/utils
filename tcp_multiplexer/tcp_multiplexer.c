/***************************************************************************
 *   This file is part of the 'utils' projects                             *
 *                                                                         *
 *   'utils' projects                                                      *
 *                                                                         *
 *   Copyright (C) 2006, 2008 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://svn.ngsoft-fr.com/trac/utils/                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/                                                        

#include "tcp_multiplexer.h"

#include <stdio.h>
#include <signal.h>
#include <string.h>


#include "assert.h"

#include "bool.h"
#include "misc.h"
#include "version.h"

#include <unistd.h>


#include <arpa/inet.h>


int send_request(int fd, unsigned char type, uint16_t id, uint16_t data_len, char * sdata)
{

    c_assert(data_len <= RT_SAFE_SIZE);

    char data[RT_BUFF];

    /* HEADER*/
    data[0] = RT_MAGIC;
    data[1] = type;
    *((uint16_t*)(data + 2)) = htons(id);
    *((uint16_t*)(data + 4)) = htons(data_len);

    /* DATA */
    size_t to_send = REQ_HEADER_SIZE;

    if(data_len)
    {
	to_send += data_len;

	int i;
	int s  = REQ_HEADER_SIZE + data_len;

	for(i = REQ_HEADER_SIZE; i < s; ++i)
	    data[i] = sdata[i];

	/*int i;
	int r = REQ_HEADER_SIZE;
	for(i = REQ_HEADER_SIZE; r < data_len; ++i, ++r)
	{
	    if(sdata[r] == RT_MAGIC)
	    {
		data[i] = RT_MAGIC;
		data[++i] = RT_MAGIC;
		to_send++;
	    }
	    else
		data[i] = sdata[r];
		}*/
    }

    return writeall(fd, data, to_send);

}

int recv_request(int fd)
{
    char data[RT_BUFF];

    if(recv(fd, data, RT_BUFF, MSG_NOSIGNAL) == -1)
	return -1;

    if(data[0] != RT_MAGIC)
    {
	dbg_printf("bad packet header\n");
	return -1;
    }

//NOT DONE
    return 0;
}


int start_tcp_tunnel_server(port_t port)
{
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


    if(listen(fdlisten, 10) == -1)
    {
        perror("listen");
	return EXIT_FAILURE;
    }

    FD_SET(fdlisten, &master);

    fdmax = fdlisten;

    while(true)
    {
        read_fds = master;

        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
	{
            perror("select");
            exit(1);
        }

	int i;
        for(i = 0; i <= fdmax; i++)
	{
            if(FD_ISSET(i, &read_fds)) 
	    {

		if(i == fdlisten)
		{
		    socklen_t addrlen = sizeof(rmaddr);
	    
		    int fd = accept(fdlisten,
				    (struct sockaddr *)&rmaddr,
				    &addrlen);
	    
		    if(fd == -1)
			perror("accept");
		    else
		    {
			FD_SET(fd, &master);
			
			if (fd > fdmax)
			    fdmax = fd;
						
			dbg_printf("incoming connection from %s on socket %d\n",
				   inet_ntoa(rmaddr.sin_addr), fd);
		    }

		}
		else
		{


		    bool closeit = false;

		    char buf[RT_SAFE_SIZE];
		    int n = recv(i, buf, RT_SAFE_SIZE, MSG_NOSIGNAL);

		    if(n <= 0)
		    {
			if(n < 0) perror("recv");
			closeit = true;
		    }
		    else
		    {
                      //process data
		    }

		    if(closeit)
		    {
			dbg_printf("socket %d closed\n", i);

			close(i);
			FD_CLR(i, &master);

		    }

		}

	    }

	}




    }


    return EXIT_SUCCESS;
}


int main()
{

    return EXIT_SUCCESS;
}

