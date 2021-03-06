/******************************************************************************
 *   btun/btund is a powerful backward tunnel creator                         *
 *                                                                            *
 *   An user guide is available on:                                           *
 *    http://svn.ngsoft-fr.com/trac/utils/wiki/BtunUserGuideEnglish           *
 *                                                                            *
 ******************************************************************************/
 
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
#include "stats.h"

#define RECV_BUFF_SIZE 768

static int server_run;
static user_pool_t * existing_users;
static channel_pool_t * channels;
static vector_t * users; /* connected users */
static stats_t stats;
static FILE * logfile;

int start_server(user_pool_t * eu, port_t port, FILE * flog, option_t options)
{
    server_run = 1;

    init_stats(&stats);

    logfile = flog;
    existing_users = eu;
    channels = create_channel_pool();
    users = create_vector(16);

    fd_set master;
    fd_set read_fds;

    struct sockaddr_in myaddr;
    struct sockaddr_in rmaddr;

    int fdmax;
    int fdlisten;

    size_t s;
    size_t i;

    int yes = 1;

    if(options & OPT_NOAUTH)
	add_channel_to_pool(channels, create_channel("::global"));

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
		    continue;
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
		perror("accept");
	    else
	    {
		stats.conn_attempt++;

		FD_SET(fd, &master);
		
		if (fd > fdmax)
		    fdmax = fd;

		channel_entry_t * ne = create_channel_entry(fd, &rmaddr.sin_addr);
		vector_add_element(users, ne);
		
		log_write(LE_CONNECTION, ne);

		if(options & OPT_NOAUTH)
		{
		    ne->channel = channel_at(channels, 0);
		    if(channel_add_user(ne->channel, ne, options) == CA_GRANTED)
			ne->step = S_AFFECTED;
		}

		dbg_printf("incoming connection from %s on socket %d\n",
			   inet_ntoa(rmaddr.sin_addr), fd);
	    }
	    
	}

	s = vector_size(users);

        for(i = 0; i < s; i++)
	{
	    channel_entry_t * e = get_entry_at(users, i);

            if(FD_ISSET(e->fd, &read_fds))
	    {

		bool closeit = false;

		char buf[RECV_BUFF_SIZE];
		int n = recv(e->fd, buf, RECV_BUFF_SIZE, MSG_NOSIGNAL);

		if(n <= 0)
		{
		    if(n < 0) perror("recv");
		    closeit = true;
		}
		else
                    closeit = !process_incoming_data(buf, n, e, &master);

		if(closeit)
		{

		    channel_entry_t * next = e;
		    while(next)
		    {
			dbg_printf("socket %d closed\n", next->fd);

			close(next->fd);
			FD_CLR(next->fd, &master);

			size_t next_index = index_from_entry(next);

			if(next_index != (size_t)-1)
			{
			    vector_del_element_at(users, next_index);
			    s--;
			    if(next_index <= i)
				i--;
			}

			next = remove_user(next);

		    }

		    fdmax = get_highest_fd(fdlisten);

		}


            }
        }

    }

    s = vector_size(users);
    for(i = 0; i < s; i++)
    {
	channel_entry_t * e = get_entry_at(users, i);
	close(e->fd);
    }
    close(fdlisten);

    free_channel_pool(channels);

    s = vector_size(users);

    for(i = 0; i < s; ++i)
    {
	channel_entry_t * e = get_entry_at(users, i);
	close(e->fd);
	if(!e->channel)
	    free_channel_entry(e);
    }
    free_vector(users, 0);

    dbg_printf("server halted\n");

    return EXIT_SUCCESS;
}

channel_entry_t * remove_user(channel_entry_t * e)
{

    log_write(LE_USER_LOGOUT, e);

    channel_t * c = e->channel;

    if(c)
    {
	if(c->master == e)
	    c->master = NULL;

	channel_del_user_from_channel(c, e); /* free e also*/

	size_t count = channel_user_count(c);

	if(count == 0) /*nobody left */
	    del_channel_from_pool(channels, c); /* free c also*/
	else if(count == 1 && (c->opts & OPT_AUTOCLOSE) ) /* 1 person left & we'have 
							     an autoclose channel */
	    return  channel_get_user_at(c, 0);

	else if( (c->opts & OPT_MASTER) && !c->master) /* This was a master/slave channel
							  but master is gone
							*/
	    return  channel_get_user_at(c, count - 1);


    }
    else
	free_channel_entry(e);

    return NULL;

}


bool process_incoming_data(char * buf, int n, channel_entry_t * e, fd_set * fs)
{

    c_assert(e);

    size_t k;
    unsigned char rep, opt;
    char * str;

    switch(e->step)
    {
    case S_WAIT_LOGIN:

	str = NULL;

	if(n >= USER_MAX_LOGIN_SIZE) /* login full or too long*/
	{
	    buf[USER_MAX_LOGIN_SIZE - 1] = '\0';
	    str = buf;
	}
	else /* login shorter than max*/
	{
	    if(buf[n - 1] == '\0') /* assume we have the complete login*/
		str = buf;
	    else
		return false;
	}

	c_assert(str);

	dbg_printf("user on socket `%d' pretends to be `%s', sending challenge...\n",
		   e->fd, str);

	/* if client provide an invalid login, we do not tell him now */
	e->user = get_user_from_name(existing_users, str);

	e->challenge = create_challenge();

	e->step = S_WAIT_CHALLENGE;

	k = CHALLENGE_SIZE;
	if(sendall(e->fd, e->challenge, &k) == -1)
	{
	    dbg_printf("send failed for `%d'\n", e->fd);
	    return false;
	}
 
	break;

    case S_WAIT_CHALLENGE:

	if(!e->challenge) /* protocol violation */
	    return false;

	rep = UA_GRANTED;

	if(n == MD5_SIZE && e->user)
	{
	    MD5_CTX_ppp m;
	    challenge_answer(e->challenge, e->user->passphrase, &m);

	    for(k = 0; k < MD5_SIZE; ++k)
		if( (unsigned char)(buf[k]) != m.digest[k] )
		{
		    rep = UA_DENIED;
		    break;
		}

	}
	else
	    rep = UA_DENIED;

	free(e->challenge);
	e->challenge = NULL;

	k = 1;
	if(sendall(e->fd, &rep, &k) == -1)
	{
	    dbg_printf("send failed for `%d'\n", e->fd);
	    return false;
	}

	e->step = S_WAIT_CHANNEL;

	if(rep == UA_DENIED)
	{
	    dbg_printf("user on socket `%d': challenge failed.\n", e->fd);
	}
	else
	{
	    dbg_printf("user on socket `%d': authentified.\n", e->fd);
	}

	return (rep == UA_GRANTED);

    case S_WAIT_CHANNEL:

	str = NULL;

	if(n >= USER_MAX_CHANNEL_SIZE) /* channel name full or too long*/
	{
	    opt = (unsigned char)buf[USER_MAX_CHANNEL_SIZE - 1];
	    buf[USER_MAX_CHANNEL_SIZE - 1] = '\0';
	    str = buf;
	}
	else /* channel name shorter than max*/
	{
	    opt = (unsigned char)buf[n - 1];
	    if(buf[n - 2] == '\0')
		str = buf;
	    else
		return false;
	}

	c_assert(str); /* should never fail */

	dbg_printf("user on socket `%d' want to join the channel named `%s' (%d)\n",
		   e->fd, str, opt);

	rep = CA_GRANTED;

	e->channel = channel_from_name(channels, str);

	if(!e->channel) /* channel does not exist, try to create it*/
	{
	    e->channel = create_channel(str);

	    if(!add_channel_to_pool(channels, e->channel))
	    {
		rep = CA_TOO_MUCH_CHANNEL;
		e->channel = NULL;
		free_channel(e->channel);
	    }

	}

	if( e->channel )
	    rep = channel_add_user(e->channel, e, opt);

	e->step = S_AFFECTED;

	k = 1;
	if(sendall(e->fd, &rep, &k) == -1)
	{
	    dbg_printf("send failed for `%d'\n", e->fd);
	    return false;
	}

	dbg_printf("channel connection/creation for `%d':%d\n", e->fd, rep);

	if(rep == CA_GRANTED)
	{
	    stats.conn_success++;
	    log_write(LE_USER_LOGIN, e);
	}


	return (rep == CA_GRANTED);

    case S_AFFECTED:
    {
	size_t i;
	size_t count;

	channel_t * c = e->channel;

	if(!c /*|| (!e->user)*/) /* protocol violation */
	    return false;

	dbg_printf("socket `%d' want to dispatch some data...\n", e->fd);

	e->recv += (size_t)n;
	stats.recv += (size_t)n;

	k = channel_user_count(c);

	if(c->master == NULL || c->master == e) 
	{
	    for(i = 0; i < k; i++)
	    {
		channel_entry_t * ce = channel_get_user_at(c, i);		
		if(ce != e && FD_ISSET(ce->fd, fs))
		{
		    count = n;
		    dbg_printf("socket `%d': sending data to `%d'...\n", e->fd, ce->fd);
		    ce->sent += count;
		    stats.sent += count;
		    if(sendall(ce->fd, buf, &count) == -1)
		    {
			perror("send");
			return false;
		    }
		    
		}
	    }

	}
	else
	{
	    count = n;
	    if(FD_ISSET(c->master->fd, fs))
	    {
		dbg_printf("socket `%d': sending data to master `%d'...\n", e->fd, c->master->fd);
		c->master->sent += count;
		stats.sent += count;
		if(sendall(c->master->fd, buf, &count) == -1)
		{
		    perror("send");
		    return false;
		}
	    }

	}

    }
    break;

    }


    return true;
}

int get_highest_fd(int fdlist)
{
    size_t s = vector_size(users);
    size_t i;

    int r = fdlist;

    for(i = 0; i < s; i++)
    {
	channel_entry_t * e = get_entry_at(users, i);
	if(e->fd > r)
	    r = e->fd;
    }

    return r;
}

size_t index_from_entry(channel_entry_t * e)
{
    size_t s = vector_size(users);
    size_t i;
    for(i = 0; i < s; i++)
    {
	if( get_entry_at(users, i) == e )
	    return i;
    }

    return (size_t)-1;

}

void stop_server()
{
    server_run = 0;
}


void print_server_status()
{
    puts("btund status:");
    /*
      puts("\n\nAccount list:");
      print_user_pool(existing_users, stdout);
    */
    puts("\n\nConnected users:");
    print_entry_vector(users, stdout, true);
    puts("\n\nChannel list:");
    print_channel_pool(channels, stdout);
    puts("\n\nServer statistics:");
    print_stats(&stats, stdout);
    puts("\nbtund status end");
    fflush(stdout);
}


void log_write(log_event_t le, channel_entry_t * e)
{
    c_assert(e);

    if(!logfile) return;



    time_t t = time(NULL);
    struct tm * date =  localtime(&t);

    fprintf(logfile, "[%02d:%02d:%02d %02d/%02d/%4d] user %s%s%s (%d) ",
	    date->tm_hour,
	    date->tm_min,
	    date->tm_sec,
	    date->tm_mday,
	    date->tm_mon + 1,
	    date->tm_year + 1900,
	    e->user ? e->user->login : "",
	    e->user ? "@" : "",
	    inet_ntoa(e->ip), e->fd
	);

    switch(le)
    {
    case LE_CONNECTION:
	fputs("connected.\n", logfile);
	break;
    case LE_USER_LOGIN:
	fprintf(logfile, "logged in and has joined channel `%s'.\n", 
		e->channel ? e->channel->name : "???"
	    );
	break;
    case LE_USER_LOGOUT:
	fputs("gone.\n", logfile);
	break;
    }


    fflush(logfile);
}
