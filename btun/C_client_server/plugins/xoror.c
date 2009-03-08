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

#include <string.h>
#include "common.h"
#include "../../../xoror/xoror.h"


typedef struct
{
    cryptor in;
    cryptor out;
    char * pass;
} xoror_stream_t;

int bt_plugin_init(plugin_info_t * p)
{
    p->name = "xoror";
    p->desc = "xor encryptor plugin";
    p->author = "ng";
    p->version = LAST_ALGO_VERSION;

    char * pass = "xoror test key";
    int key = 815;

    if(p->argc > 0)
    {
	pass = p->argv[0];

	if(p->argc > 1)
	    key = atoi(p->argv[1]);
	else if(pass[0] && pass[1])
	    key = pass[0] * pass[1];
    }

    xoror_stream_t * st = (xoror_stream_t*)malloc(sizeof(xoror_stream_t));
    if(!st)
	return 0;

    st->pass = strdup(pass);

    cryptor_init(&st->in, st->pass, key);
    cryptor_init(&st->out, st->pass, key);

    p->data = st;

    return (p->data != NULL);
}


void bt_plugin_destroy(plugin_info_t * p)
{
    xoror_stream_t * st = (xoror_stream_t*)p->data;
    free(st->pass);
}

size_t bt_plugin_encode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    xoror_stream_t * st = (xoror_stream_t*)p->data;
    encrypt_data(in, in, &st->in, s);

    *out = in;

    return s;
}

size_t bt_plugin_decode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    xoror_stream_t * st = (xoror_stream_t*)p->data;
    encrypt_data(in, in, &st->out, s);

    *out = in;

    return s;
}


