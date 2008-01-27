#include "common.h"


size_t next_power_of_two(size_t k)
{
    size_t i;
    k--;
    for (i = 1;  i < sizeof(k) * 8; i *= 2)
	k = k | k >> i;

    return k + 1;
}

/* ensure that the buffer is big enough */
void ensure_buffer_size(plugin_info_t * p, size_t s)
{
    size_t old = s;
    s = next_power_of_two(s);

    if(s < MIN_BUF_SIZE)
	s = MIN_BUF_SIZE;

    if(!p->buffer) /* first time */
    {
	p->buffer_size = s;
	p->buffer = (char*)malloc(s);
    }
    else if(s > p->buffer_size) /* damn! we need a larger buffer */
    {
	p->buffer_size = s;
	p->buffer = (char*)realloc(p->buffer, s);
    }

}

