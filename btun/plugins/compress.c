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

typedef struct
{
    z_stream cpr;
    z_stream uncpr;
} nioz_stream_t;

int bt_plugin_init(plugin_info_t * p)
{
    p->name = "compress";
    p->desc = "zlib compression plugin";
    p->author = "ng";
    p->version = 1;

    int level = 9;
    nioz_stream_t * st = (nioz_stream_t*)malloc(sizeof(nioz_stream_t));
    if(!st)
	return 0;

    st->cpr.zalloc = Z_NULL;
    st->cpr.zfree = Z_NULL;
    st->cpr.opaque = Z_NULL;

    st->uncpr.zalloc = Z_NULL;
    st->uncpr.zfree = Z_NULL;
    st->uncpr.opaque = Z_NULL;

    /* header reading will be dona later */
    st->uncpr.next_in = Z_NULL;
    st->uncpr.avail_in = 0;

    if(p->argc)
    {
	level = atoi(p->argv[0]);
	if(level < 0 || level > 9)
	{
	    fputs("compress: invalid compression level\n", stderr);
	    return 0;
	}
    }

    if(deflateInit(&st->cpr, level) != Z_OK)
	return 0;

    if(inflateInit(&st->uncpr) != Z_OK)
	return 0;

    p->data = st;
    return 1;

}


void bt_plugin_destroy(plugin_info_t * p)
{
    nioz_stream_t * st = (nioz_stream_t*)p->data;

    printf("GAIN CPR=%ld\n", st->cpr.total_in - st->cpr.total_out);
    printf("GAIN UNCPR=%ld\n", st->uncpr.total_out - st->uncpr.total_in);
    free(p->buffer);


    deflateEnd(&st->cpr);
    inflateEnd(&st->uncpr);
    free(st);
}

size_t bt_plugin_encode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    /*
      Normaly, output buffer must be at least input * 1.001 + 12.
      We take input * 1.01 + 12 to be sure to process all data in one time.
     */
    ensure_buffer_size(p, (size_t)(((float)s) * 1.01 + 12.00) + 1);
    if(!p->buffer)
	return BT_ERROR;

    nioz_stream_t * st = (nioz_stream_t*)p->data;

    st->cpr.next_in = in;
    st->cpr.avail_in = s;

    st->cpr.next_out = p->buffer;
    st->cpr.avail_out = p->buffer_size;

    int status = deflate(&st->cpr, Z_SYNC_FLUSH);

    if(status != Z_OK && status != Z_STREAM_END)
	return BT_ERROR;


    *out = p->buffer;

    return p->buffer_size - st->cpr.avail_out;

}


size_t bt_plugin_decode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    nioz_stream_t * st = (nioz_stream_t*)p->data;

    int status;
    size_t ns = s;

    uLong tot = st->uncpr.total_out;

    st->uncpr.next_in = in;
    st->uncpr.avail_in = s;

    size_t last_done = 0;

    do
    {
	ensure_buffer_size(p, ns * 2);
	if(!p->buffer)
	    return BT_ERROR;
	else
	    ns = p->buffer_size;


        /* p->buffer address may change */
	st->uncpr.next_out = p->buffer + last_done;
	st->uncpr.avail_out = ns - last_done;

	status = inflate(&st->uncpr, Z_SYNC_FLUSH);

	last_done += ns;
    }
    while(st->uncpr.avail_out == 0 && status != Z_DATA_ERROR);

    if(status != Z_OK && status != Z_STREAM_END)
	return BT_ERROR;


    *out = p->buffer;

    tot = st->uncpr.total_out - tot;

    return (size_t)tot;
}


