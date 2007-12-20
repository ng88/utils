#ifndef CHANNEL_H
#define CHANNEL_H

#include "vector.h"

typedef struct
{
    int fd;
    char * login;
    char * passphrase;

} channel_entry_t;

typedef struct
{
    /* vector of channel_entry_t * */
    vector_t entries;

} channel_t;


typedef struct
{
    /* vector of channel_t * */
    vector_t channels;

} channel_pool_t;




channel_pool_t * create_channel_pool();

void free_channel_pool(channel_pool_t * p);



channel_t * create_channel();

void channel_add_user(channel_t * c, channel_entry_t * e);

#define channel_user_count(c) \
             vector_size(c->entries);

#define channel_entry_at(c) \
            (channel_entry_t *)vector_size(c->entries);

void channel_del_user(

channel_entry_t * channel_entry_at(channel_t * c, size_t i);

void free_channel(channel_t * c);



channel_entry_t * create_channel_entry(int fd, char * login, char * passphrase);

void free_channel_entry(channel_entry_t * e);




#endif
