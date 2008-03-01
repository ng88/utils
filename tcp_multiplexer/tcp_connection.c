
#include "tcp_connection.h"
#include "assert.h"

tcp_connection_t * create_tcp_infos(int fd, int id)
{
    tcp_connection_t * ret =
	(tcp_connection_t *)malloc(sizeof(*ret));

    c_assert(ret);

    ret->fd = fd;
    ret->id = id;
    ret->deleted = false;

    return ret;
}

void free_tcp_infos(tcp_connection_t * e)
{
    free(e);
}

tcp_connection_t * get_infos_from_id(vector_t * v, int id)
{
    c_assert(v);

    size_t i;
    size_t s = tcp_infos_count(v);

    for(i = 0; i < s; i++)
    {
	tcp_connection_t * e = get_tcp_infos(v, i);
	if(e->id == id)
	    return e;
    }

    return NULL;
}



