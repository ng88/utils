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


#include "channel.h"
#include "server.h"
#include "assert.h"

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


/*********  CHANNEL  **********/


channel_t * create_channel(char * name)
{
   channel_t * r = 
	(channel_t *)malloc(sizeof(channel_t));

    c_assert2(r, "malloc failed");

    r->entries = create_vector(8);

    return r;
}


void channel_del_user_at(channel_t * c, size_t i)
{
    c_assert(c);

    free_channel_entry(vector_get_element_at(c->entries, i));
    vector_del_element_at(c->entries, i);
}


int channel_add_user(channel_t * c, channel_entry_t * e)
{
    c_assert(c);

    if(channel_user_count(c) >= SERVER_MAX_USER_PER_CHANNEL)
	return 0;
    else
    {
	vector_add_element(c->entries, e);
	return 1;
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



/*********  CHANNEL ENTRY  **********/

channel_entry_t * create_channel_entry(int fd, user_t * user)
{
   channel_entry_t * r = 
	(channel_entry_t *)malloc(sizeof(channel_entry_t));

    c_assert2(r, "malloc failed");

    r->fd = fd;
    r->user = user;

    return r;
}

void free_channel_entry(channel_entry_t * e)
{
    c_assert(e);

    free(e);
}

