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
#include <stdio.h>
#include <string.h>

int bt_plugin_init(plugin_info_t * p)
{
    p->name = "tee";
    p->desc = "dump tunnel traffic to disk";
    p->author = "ng";
    p->version = 1;

    if(p->argc == 0)
    {
	fputs("tee: missing file argument.\n", stderr);
	return 0;
    }

    p->data = (!strcmp(p->argv[0], "-")) ? stdout : fopen(p->argv[0], "wb");
    if(!p->data)
    {
	fprintf(stderr, "tee: unable to open `%s'!\n", p->argv[0]);
	return 0;
    }

   /* init success */
    return 1;
}

void bt_plugin_destroy(plugin_info_t * p)
{
    fclose((FILE*)p->data);
}


size_t bt_plugin_encode(plugin_info_t * p, char * in, size_t s, char ** out)
{

   /* dump input traffic */
    fwrite(in, 1, s, (FILE*)p->data);

    *out = in;
    return s;
}


size_t bt_plugin_decode(plugin_info_t * p, char * in, size_t s, char ** out)
{

   /* dump output traffic */
    fwrite(in, 1, s, (FILE*)p->data);

    *out = in;
    return s;
}


