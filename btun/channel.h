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
    vector_t * entries;
    char * name;

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

void free_channel_pool(channel_pool_t * p);



/*********  CHANNEL  **********/

channel_t * create_channel(char * name);

int channel_add_user(channel_t * c, channel_entry_t * e);

#define channel_user_count(c)   (vector_size((c)->entries))

#define channel_get_user_at(c, i)  \
         ((channel_entry_t *)vector_get_element_at((c)->entries, (i)))

void channel_del_user_at(channel_t * c, size_t i);

void free_channel(channel_t * c);


/*********  CHANNEL ENTRY  **********/

channel_entry_t * create_channel_entry(int fd, char * login, char * passphrase);

void free_channel_entry(channel_entry_t * e);




#endif
