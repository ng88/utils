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
#include <zlib.h>


void bt_plugin_init(plugin_info_t * p)
{
    p->name = "compress";
    p->desc = "zlib compression plugin";
    p->author = "ng";
    p->version = 1;
}


void bt_plugin_destroy(plugin_info_t * p)
{
    if(p->buffer)
	free(p->buffer);
}

/* ensure that the buffer is big enough */
void ensure_buffer_size(plugin_info_t * p, size_t s)
{
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

size_t bt_plugin_encode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    /*
      Normaly, output buffer must be at least input * 1.001 + 12.
      We take input * 1.01 + 12 to avoid any risk of buffer overflow.
     */
    ensure_buffer_size(p, (size_t)(((float)s) * 1.01 + 12.00) + 1);
    if(!p->buffer)
	return BT_ERROR;


    uLongf ns = p->buffer_size;
    if(compress2(p->buffer, &ns, in, s, 9) != Z_OK)
	return BT_ERROR;

    *out = p->buffer;

    return ns;

}

/* It remains a bug that get the client crash sometime...
 */

size_t bt_plugin_decode(plugin_info_t * p, char * in, size_t s, char ** out)
{

    ensure_buffer_size(p, s * 2);
    if(!p->buffer)
	return BT_ERROR;


    uLongf ns = p->buffer_size;
    int ret;

    do
    {/* I think there is someting wrong here... to be checked */
	ret = uncompress(p->buffer, &ns, in, s);
	ensure_buffer_size(p, p->buffer_size * 2);
    }
    while(ret == Z_BUF_ERROR);


    if(ret != Z_OK)
	return BT_ERROR;

    *out = p->buffer;

    return ns;
}


