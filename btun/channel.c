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


#include "channel.h"
#include "server.h"
#include "assert.h"

#include <arpa/inet.h>
#include <string.h>


/*********  CHANNEL POOL  **********/

channel_pool_t * create_channel_pool()
{
    channel_pool_t * r = 
	(channel_pool_t *)malloc(sizeof(channel_pool_t));

    c_assert2(r, "malloc failed");

    r->channels = create_vector(8);

    return r;
}


channel_t * channel_from_name(channel_pool_t * p, char * name)
{
    c_assert(p);
    c_assert(name);

    size_t i;
    size_t s = channel_count(p);

    for(i = 0; i < s; ++i)
    {
	channel_t * c = channel_at(p, i);
	if(!strcmp(c->name, name))
	    return c;
    }

    return NULL;

}

bool add_channel_to_pool(channel_pool_t * p, channel_t * c)
{
    c_assert(p && c);

    if(channel_count(p) >= SERVER_MAX_CHANNEL)
	return false;

    vector_add_element(p->channels, c);

    return true;
}

void del_channel_at(channel_pool_t * c, size_t i)
{
    c_assert(c);

    free_channel(vector_get_element_at(c->channels, i));
    vector_del_element_at(c->channels, i);
}

void del_channel_from_pool(channel_pool_t * c, channel_t * e)
{
    c_assert(c);

    size_t s = channel_count(c);
    size_t i;

    for(i = 0; i < s; ++i)
	if(channel_at(c, i) == e)
	{
	    del_channel_at(c, i); /* free e */
	    break;
	}
}

void free_channel_pool(channel_pool_t * p)
{
    c_assert(p);

    size_t i;
    size_t s = channel_count(p);

    for(i = 0; i < s; ++i)
	free_channel(channel_at(p, i));

    free_vector(p->channels, 0);

    free(p);
}


void print_channel_pool(channel_pool_t * p, FILE * f)
{
    size_t i;
    size_t s = channel_count(p);

    fputs("---- channel table -----------\n", f);

    for(i = 0; i < s; ++i)
    {
	channel_t * c = channel_at(p, i);
	fprintf(f, "Channel %s ", c->name);

	if(c->master)
	    fprintf(f, " - master is %s:%d", c->master->user->login, c->master->fd);

	if( (c->opts & OPT_AUTOCLOSE) )
	    fputs(" - autoclose", f);

	if( (c->opts & OPT_UNRESTRICTED) )
	    fputs(" - unrestricted:\n", f);
	else
	    fputs(" - restricted:\n", f);

	print_entry_vector(c->entries, f, false);
    }

    fprintf(f, "---- %3u channels ------------\n", s);
}


/*********  CHANNEL  **********/


channel_t * create_channel(char * name)
{
   channel_t * r = 
	(channel_t *)malloc(sizeof(channel_t));

    c_assert2(r, "malloc failed");

    r->entries = create_vector(8);
    r->name = strdup(name);
    r->master = NULL;

    return r;
}


void channel_del_user_at(channel_t * c, size_t i)
{
    c_assert(c);

    free_channel_entry(vector_get_element_at(c->entries, i));
    vector_del_element_at(c->entries, i);
}

void channel_del_user_from_channel(channel_t * c, channel_entry_t * e)
{
    c_assert(c);

    size_t s = channel_user_count(c);
    size_t i;

    for(i = 0; i <  s; ++i)
	if(channel_get_user_at(c, i) == e)
	{
	    channel_del_user_at(c, i); /* free e */
	    break;
	}
}


unsigned char channel_add_user(channel_t * c, channel_entry_t * e, option_t opt)
{
    c_assert(c);

    e->channel = NULL;

    size_t cc = channel_user_count(c);

    if(cc >= SERVER_MAX_USER_PER_CHANNEL)
	return CA_TOO_MUCH_USER;
    else
    {

	/* we join an existing channel */
	if(cc > 0)
	{
	    if( (opt & OPT_MASTER) )
		return CA_CANT_BE_MASTER;

	    if( (opt & OPT_UNRESTRICTED) )
		return CA_CANT_CHPERM;

	    if( (opt & OPT_AUTOCLOSE) )
		return CA_CANT_CHPERM;

	    if( !(c->opts & OPT_UNRESTRICTED) && channel_get_user_at(c, 0)->user != e->user )
		return CA_DENIED;
	}
	else /* we created the channel */
	{
	    if( (opt & OPT_MASTER) )
		c->master = e;

	    c->opts = opt;

	}

	vector_add_element(c->entries, e);
	e->channel = c;


	return CA_GRANTED;
    }


}


void free_channel(channel_t * c)
{
    c_assert(c);

    free(c->name);

    size_t i;
    size_t s = channel_user_count(c);

    for(i = 0; i < s; ++i)
	free_channel_entry(channel_get_user_at(c, i));

    free_vector(c->entries, 0);

    free(c);
}

void print_entry_vector(vector_t * v, FILE * f, bool showsep)
{
    c_assert(v);

    size_t i;
    size_t s = vector_size(v);

    if(showsep)
	fprintf(f, "---- entry table -------------\n");
    
    for(i = 0; i < s; ++i)
    {
	channel_entry_t * e = get_entry_at(v, i);
	fprintf(f, "%s:%d@%s (",
		e->user ? e->user->login : "unknown",
		e->fd, inet_ntoa(e->ip)
		);

	if(showsep)
	    fprintf(f, "%s",
		    e->channel ? e->channel->name : "unknown"
		  );
	else
	    fprintf(f, "sent=%ub, recv=%ub",
		    e->recv, e->sent
		    /* what user sent is what server received
		       and vice versa */
		   );

	puts(")");
    }

    if(showsep)
	fprintf(f, "---- %3u entries -------------\n", s);
}



/*********  CHANNEL ENTRY  **********/

channel_entry_t * create_channel_entry(int fd, struct in_addr ip)
{
   channel_entry_t * r = 
	(channel_entry_t *)malloc(sizeof(channel_entry_t));

    c_assert2(r, "malloc failed");

    r->fd = fd;
    r->user = NULL;
    r->step = S_WAIT_LOGIN;
    r->channel = NULL;
    r->challenge = NULL;
    r->sent = 0;
    r->recv = 0;
    r->ip = ip;
    return r;
}

void free_channel_entry(channel_entry_t * e)
{
    c_assert(e);

    if(e->challenge)
	free(e->challenge);

    free(e);
}

