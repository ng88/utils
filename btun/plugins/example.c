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
#include <ctype.h>

/* This is a plugin example
   It simply put the input in upper case.

   It does not decode.
 */

/** We have to fill some field...
 */
int bt_plugin_init(plugin_info_t * p)
{
    p->name = "uppercase";
    p->desc = "a WONDERFUL plugin :)";
    p->author = "ng";
    p->version = 1;

    /* plugin can have argument
       they are stored in p->argc & p->argv
 
    int i;
    for(i = 0; i < p->argc;  ++i)
	printf("  plugin argv[%d] = `%s'\n", i, p->argv[i]);

    */

   /* init success */
    return 1;
}

/** Free resources
 */
void bt_plugin_destroy(plugin_info_t * p)
{
    /* Free the buffer, if it has been used */
    if(p->buffer)
	free(p->buffer);
}

/** Encode the output stream (sent to server)
 *   p contains the plugin information
 *   in is the input buffer
 *   s is the length of the data in the input buffer
 *   out must be set to our output buffer
 * And we have to return the size of the data it contains.
 */
size_t bt_plugin_encode(plugin_info_t * p, char * in, size_t s, char ** out)
{

   /* We dont need an output buffer larger that the input buffer
    and the transformation is easy.

    So we just change the input buffer and return it.

    For an example with an ouput buffer, see the compress plugin.
   */

    size_t i;

    for(i = 0; i < s; ++i)
	in[i] = toupper(in[i]);


    /* set the output buffer */
    *out = in;

    return s;
}

/** Same thing as encode, but decode input stream (received from server)
 */
size_t bt_plugin_decode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    /* our plugin does not decode */

    *out = in;

    return s;
}


