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
#include "../../xoror/xoror.h"


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

    p->data = cryptor_new(pass, key);

    return (p->data != NULL);
}


void bt_plugin_destroy(plugin_info_t * p)
{
    cryptor_free((cryptor*)p->data);
}

size_t bt_plugin_encode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    encrypt_data(in, in, (cryptor*)p->data, s);

    *out = in;

    return s;
}

size_t bt_plugin_decode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    encrypt_data(in, in, (cryptor*)p->data, s);

    *out = in;

    return s;
}


