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


#include "common.h"
#include <zlib.h>



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


