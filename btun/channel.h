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

#ifndef CHANNEL_H
#define CHANNEL_H

#include "vector.h"
#include "user.h"
#include "bool.h"
#include <netinet/in.h>

typedef enum
{
    S_WAIT_LOGIN = 0,
    S_WAIT_CHALLENGE = 1,
    S_WAIT_CHANNEL = 2,
    S_AFFECTED = 3,
} step_t;

struct _channel_t;

typedef struct
{
    /** Socket fd */
    int fd;
    /** Auth step */
    step_t step;
    /** User information */
    user_t * user;
    /** Channel where the user is */
    struct _channel_t * channel;
    /** The challenged proposed by server */
    char * challenge;
    /** Amount of byte sent to this user */
    size_t sent;
    /** Amount of byte received from this user */
    size_t recv;
    /** User IP*/
    struct in_addr ip;
} channel_entry_t;


typedef struct _channel_t
{
    /* vector of channel_entry_t * */
    vector_t * entries;
    channel_entry_t * master;
    char * name;
    option_t opts;
} channel_t;


typedef struct
{
    /* vector of channel_t * */
    vector_t * channels;

} channel_pool_t;



/*********  CHANNEL POOL  **********/

channel_pool_t * create_channel_pool();

channel_t * channel_from_name(channel_pool_t * p, char * name);

#define channel_count(p)   vector_size((p)->channels)

#define channel_at(p, i)  \
            ((channel_t*)vector_get_element_at((p)->channels, (i)))

bool add_channel_to_pool(channel_pool_t * p, channel_t * c);

void del_channel_at(channel_pool_t * c, size_t i);

void del_channel_from_pool(channel_pool_t * c, channel_t * e);

void free_channel_pool(channel_pool_t * p);

void print_channel_pool(channel_pool_t * p, FILE * f);

/*********  CHANNEL  **********/

channel_t * create_channel(char * name);

unsigned char channel_add_user(channel_t * c, channel_entry_t * e, option_t opt);

#define channel_user_count(c)   (vector_size((c)->entries))

#define channel_get_user_at(c, i)  \
         ((channel_entry_t *)vector_get_element_at((c)->entries, (i)))

void channel_del_user_at(channel_t * c, size_t i);

void channel_del_user_from_channel(channel_t * c, channel_entry_t * e);

void free_channel(channel_t * c);

#define get_entry_at(v, i) \
           ((channel_entry_t*)vector_get_element_at((v), (i)))

void print_entry_vector(vector_t * v, FILE * f, bool showsep);


/*********  CHANNEL ENTRY  **********/

channel_entry_t * create_channel_entry(int fd, struct in_addr * ip);

void free_channel_entry(channel_entry_t * e);




#endif
