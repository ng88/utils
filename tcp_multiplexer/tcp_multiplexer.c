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
#include <netdb.h>

#include "assert.h"

#include "bool.h"
#include "misc.h"
#include "version.h"

#include <unistd.h>


#include <arpa/inet.h>



#define SERVER_BACKLOG 5

static volatile bool run = false;
static vector_t * connections = NULL;
static char * addr = NULL;
static port_t port = 0;
static bool listen_mode = false;
static struct hostent * h = NULL;
static fd_set master;
static int fdmax;

void stop()
{
    run = false;
}

int tcp_mux(int fdin, int fdout)
{
    /* 
       ecoute sur fdin
       ecoute sur port
       select
       parse fdin -> canal[i]
       canal[i] met en forme -> fdout
       (send data & ordre de connection)

     */
    connections = create_vector(16);

    run = true;

    fd_set read_fds;

    struct sockaddr_in myaddr;
    struct sockaddr_in rmaddr;

    int fdlisten = -1;

    size_t s;
    size_t i;

    int yes = 1;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    fdmax = fdin;


    if(listen_mode) /* LISTENER MODE */
    {

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

	if(fdlisten > fdmax)
	    fdmax = fdlisten;
    }
    else  /* CONNECTOR MODE */
    {
	if((h = gethostbyname(addr)) == NULL)
	{
	    herror("gethostbyname");
	    return EXIT_FAILURE;
	}
	
    }

    FD_SET(fdin, &master);


    dbg_printf("server started\n");

    while(run)
    {
        read_fds = master;
        if(select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
	{
            perror("select");
	    return EXIT_FAILURE;
        }

	if(FD_ISSET(fdin, &read_fds))
	{
	    int n = recv_request(fdin);

	    if(n <= 0)
	    {
		if(n < 0) perror("read");
		run = false;
		dbg_printf("input closed\n");
		break;
	    }
	}
	
	if(listen_mode && FD_ISSET(fdlisten, &read_fds))
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

		tcp_connection_t * e = create_tcp_infos(fd, fd);
		add_tcp_infos(connections, e);

		send_request(e, fdout, RT_CONNECT, 0, NULL);

		dbg_printf("incoming connection from %s on socket %d\n",
			   inet_ntoa(rmaddr.sin_addr), fd);
	    }
	    
	}

        for(i = 0; i < tcp_infos_count(connections); i++)
	{
	    tcp_connection_t * e = get_tcp_infos(connections, i);

	    if(e->deleted)
	    {
		FD_CLR(e->fd, &master);
		vector_del_element_at(connections, i);
		free_tcp_infos(e);
		i--;
		continue;
	    }

            if(FD_ISSET(e->fd, &read_fds))
	    {

		char buf[RT_BUFF];
		int n = recv(e->fd, buf, RT_BUFF, MSG_NOSIGNAL);

		if(n <= 0)
		{
		    if(n < 0)
			perror("recv");

		    send_request(e, fdout, RT_CLOSE, 0, NULL);
		    close(e->fd);
		    FD_CLR(e->fd, &master);

		    vector_del_element_at(connections, i);
		    i--;

		    dbg_printf("closed %d\n", e->fd);

		    free_tcp_infos(e);
		}
		else
		{
		    dbg_printf("data from %d\n", e->fd);
		    send_request(e, fdout, RT_DATA, n, buf);
		}

            }
        }

    }

    s = tcp_infos_count(connections);
    for(i = 0; i < s; i++)
    {
	tcp_connection_t * e = get_tcp_infos(connections, i);
	send_request(e, fdout, RT_CLOSE, 0, NULL);
	close(e->fd);
	free_tcp_infos(e);
    }

    close(fdlisten);

    free_vector(connections, false);

    return EXIT_SUCCESS;
}

int send_request(tcp_connection_t * e, int fdout, unsigned char type, uint16_t data_len, char * sdata)
{
    c_assert(e && sizeof(uint16_t) == 2);

    /* uint16_t id = (uint16_t)(listen_mode ? e->fd : e->id);*/

    dbg_printf("send packet type=%d, id=%d, len=%d\n", type, e->id, data_len);

    /* prepare & send header */
    char buff[REQ_HEADER_SIZE];
    buff[0] = RT_MAGIC;
    buff[1] = type;
    *((uint16_t*)(buff + 2)) = htons((uint16_t)e->id);
    *((uint16_t*)(buff + 4)) = htons(data_len);

    if(writeall(fdout, buff, REQ_HEADER_SIZE) == -1)
	return -1;

    /* DATA */
    if(data_len)
	return writeall(fdout, sdata, data_len);
	
    return 0;
}

int recv_request(int fd)
{
    int n;

    /* retrieve header */
    char buff[REQ_HEADER_SIZE];
    
    if( (n = readall(fd, buff, REQ_HEADER_SIZE)) <= 0)
	return n;

    header_t header;
    header.magic = buff[0];
    header.type = buff[1];
    header.id = ntohs(*((uint16_t*)(buff + 2)));
    header.len = ntohs(*((uint16_t*)(buff + 4)));

    dbg_printf("recv packet magic=%d type=%d, id=%d, len=%d\n", 
	       header.magic, header.type, header.id, header.len);

    if(header.magic != RT_MAGIC)
    {
	dbg_printf("bad packet header\n");
	return -1;
    }

    tcp_connection_t * e = NULL;

    if(header.type != RT_CONNECT)
    {
	e = get_infos_from_id(connections, header.id);

	if(!e)
	{
	    dbg_printf("bad packet id\n");
	    return -1;
	}

    }

    switch(header.type)
    {
    case RT_CONNECT:
        {
	    int sockfd;
	    struct sockaddr_in dest_addr;

	    if( (sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1 )
	    {
		perror("socket");
		return -1;
	    }

	    dbg_printf("trying to connect to %s (%s:%u)...\n", addr, 
		       inet_ntoa(*((struct in_addr *)h->h_addr)), port);


	    dest_addr.sin_family = AF_INET;
	    dest_addr.sin_port = htons(port);
	    dest_addr.sin_addr = *((struct in_addr *)h->h_addr);
	    memset(dest_addr.sin_zero, 0, sizeof(dest_addr.sin_zero));

	    if( connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) == -1)
	    {
		perror("connect");
		return -1;
	    }

	    FD_SET(sockfd, &master);
		
	    if(sockfd > fdmax)
		fdmax = sockfd;

	    add_tcp_infos(connections, create_tcp_infos(sockfd, header.id));

	    dbg_printf("new connection %d on %d\n", header.id, sockfd);
        }
	break;
    case RT_CLOSE:
	dbg_printf("connection %d closed\n", e->fd);
	e->deleted = true;
	close(e->fd);
	break;
    case RT_DATA:
        if(header.len)
	{
	    c_assert(header.len <= RT_BUFF);

	    int s;
	    char data[RT_BUFF];
	    if( (n = readall(fd, data, header.len)) <= 0)
		return n;
	    
	    s = header.len;
	    if(sendall(e->fd, data, &s) == -1)
		return -1;
	    
	}
	break;
    default:
	dbg_printf("bad packet type\n");
	return -1;
    }

    return 1;
}

void set_addr(char * a, port_t p)
{
    addr = a;
    port = p;
    listen_mode = a ? false : true;
}

void usage(int ev)
{
    fputs("usage: tcpmux ip port\n"
	  "           connect tunnel to ip:port\n"
	  "       tcpmux port\n"
	  "           listen to port\n"
	  , stderr);
    exit(ev);
}

int main(int argc, char ** argv)
{

    switch(argc)
    {
    case 2:
	set_addr(NULL, atoi(argv[1]));
	break;
    case 3:
	set_addr(argv[1], atoi(argv[2]));
	break;
    default:
	usage(EXIT_FAILURE);
    }

    return tcp_mux(STDIN_FILENO, STDOUT_FILENO);
}

