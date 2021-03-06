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

#ifndef PLUGIN_DEF_H
#define PLUGIN_DEF_H

#include <stdlib.h>


#define BT_ERROR ((size_t)-1)

struct splugin_info_t;

typedef int (*fn_plug_init_t)(struct splugin_info_t * p);
typedef void (*fn_plug_free_t)(struct splugin_info_t * p);
typedef size_t (*fn_plug_inout_t)(struct splugin_info_t * p, char * in,
				  size_t s, char ** out);

typedef struct splugin_info_t
{
    /** Plugin name */
    char * name;
    /** Plugin short description */
    char * desc;
    /** Plugin's author name */
    char * author;
    /** Plugin version */
    unsigned short version;
    
    /** Can be used by plugin to store 
	its satus or some needed data */
    void * data;

    /** Buffer that can be used by the plugin (with ensure_buffer_size()
     *  from plugins/common.h)
     */
    char * buffer;
    size_t buffer_size;

    /** Callbacks
     */
    fn_plug_free_t destructor;
    fn_plug_inout_t encoder;
    fn_plug_inout_t decoder;

    void * module;
    
    /** Plugin arguments
     */
    int argc;
    char ** argv;

} plugin_info_t;




/*

// You must declare the following function in order to create a functional plugin:

// init plugin & return its information
// return 0 on failure
int bt_plugin_init(plugin_info_t * p);

// destroy plugin
void bt_plugin_destroy(plugin_info_t * p);

// request plugin to encode/decode data, buffer size of in & out is PLUGIN_BUFF_SIZE
// s is the number of byte to process
// return the number of byte that we must transfer from out or BT_ERROR on error
// plugin MUST have it's own output buffer and set *out to it
size_t bt_plugin_encode(plugin_info_t * p, char * in, size_t s, char ** out);
size_t bt_plugin_decode(plugin_info_t * p, char * in, size_t s, char ** out);



*/


#endif
