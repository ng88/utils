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


void bt_plugin_init(plugin_info_t * p)
{
    p->name = "compress";
    p->desc = "compression plugin";
    p->author = "Nicolas GUILLAUME";
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

    ensure_buffer_size(p, s);
    if(!p->buffer)
	return BT_ERROR;


/* TODO -- TODO -- TODO COMPRESS HERE
 */


    *out = p->buffer;

    return s;

}

size_t bt_plugin_decode(plugin_info_t * p, char * in, size_t s, char ** out)
{

    ensure_buffer_size(p, s);
    if(!p->buffer)
	return BT_ERROR;


/* TODO -- TODO -- TODO UNCOMPRESS HERE
 */


    *out = p->buffer;

    return s;
}


