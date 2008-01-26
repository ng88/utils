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

#include "../plugin_def.h"
#include "../assert.h"
#include <zlib.h>

#define MIN_BUF_SIZE 256


int bt_plugin_init(plugin_info_t * p)
{
    p->name = "compress";
    p->desc = "zlib compression plugin";
    p->author = "ng";
    p->version = 1;

    p->buffer_size = MIN_BUF_SIZE;
    p->buffer = (char*)malloc(MIN_BUF_SIZE);

    return (p->buffer != NULL);

}


void bt_plugin_destroy(plugin_info_t * p)
{
    free(p->buffer);
}

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
    dbg_printf("begin ensure_buffer_size(%p, %u)\n", p, s);

    s = next_power_of_two(s);

    dbg_printf(" -> next pow of 2 %u\n", s);

    if(!p->buffer) /* first time */
    {
	p->buffer_size = s;
	p->buffer = (char*)malloc(s);
	dbg_printf(" -> new, sized to %u\n", s);
    }
    else if(s > p->buffer_size) /* damn! we need a larger buffer */
    {
	p->buffer_size = s;
	p->buffer = (char*)realloc(p->buffer, s);
	dbg_printf(" -> resized to %u\n", s);
    }

    dbg_printf("end ensure_buffer_size(%p, %u)\n", p, s);
}

size_t bt_plugin_encode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    dbg_printf("encode(%p, %p, %u, ?)\n", p, in, s);
    /*
      Normaly, output buffer must be at least input * 1.001 + 12.
      We take input * 1.01 + 12 to avoid any risk of buffer overflow.
     */
    ensure_buffer_size(p, (size_t)(((float)s) * 1.01 + 12.00) + 1);
    if(!p->buffer)
	return BT_ERROR;

    int st;
    uLongf ns = p->buffer_size;
    if( (st=compress2(p->buffer, &ns, in, s, 9)) != Z_OK)
    {
	dbg_printf("compress2 error %d\n", st);
	return BT_ERROR;
    }

    *out = p->buffer;

    return ns;

}

/* It remains a bug that get the client crash sometime...
 */

size_t bt_plugin_decode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    dbg_printf("decode(%p, %p, %u, ?)\n", p, in, s);

    uLongf ns = s;
    int ret;

    do
    {/* I think there is someting wrong here... to be checked */
	ensure_buffer_size(p, ns * 2);

	if(!p->buffer)
	    return BT_ERROR;
	else
	    ns = p->buffer_size;

	ret = uncompress(p->buffer, &ns, in, s);


	dbg_printf("pass1 %lu\n", ns);
    }
    while(ret == Z_BUF_ERROR);

    if(ret != Z_OK)
    {
	dbg_printf("uncompress error %d\n", ret);
	return BT_ERROR;
    }

    *out = p->buffer;

    return ns;
}


