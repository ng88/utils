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

//include path
#include "../plugin_def.h"
#include "../../xoror/xoror.h"


void xoror_LTX_bt_plugin_init(plugin_info_t * p)
{
    p->name = "xoror";
    p->desc = "xor encryptor plugin";
    p->author = "Nicolas GUILLAUME";
    p->version = LAST_ALGO_VERSION;

    p->data = cryptor_new("xoror test key", 815);
}


void xoror_LTX_bt_plugin_destroy(plugin_info_t * p)
{
    cryptor_free((cryptor*)p->data);
}

size_t xoror_LTX_bt_plugin_encode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    static char * buff = NULL;
    static size_t old_size = 0;

    if(!buff) /* first time */
    {
	old_size = s;
	buff = (char*)malloc(s);
    }
    else if(s > old_size) /* damn! we need a larger buffer */
    {
	old_size = s;
	buff = (char*)realloc(buff, s);
    }

    if(!buff)
	return BT_ERROR;

    encrypt_data(in, buff, (cryptor*)p->data, s);

    *out = buff;

    return s;

}

size_t xoror_LTX_bt_plugin_decode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    return xoror_LTX_bt_plugin_encode(p, in, s, out);
}


