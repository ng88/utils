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

#ifdef BTUN_DL_PLUGIN
#include <ltdl.h>
#endif

struct splugin_info_t;


typedef enum
{
    PS_ERROR = -1,
    PS_FATAL_ERROR = 0,
    PS_SUCESS = 1,
} ps_t;



typedef void (*fn_plug_init_t)(struct splugin_info_t * p);
typedef void (*fn_plug_free_t)(struct splugin_info_t * p);
typedef ps_t (*fn_plug_inout_t)(struct splugin_info_t * p, char * in, char * out);


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

    fn_plug_free_t * destructor;
    fn_plug_inout_t * encoder;
    fn_plug_inout_t * decoder;

#ifdef BTUN_DL_PLUGIN
    lt_dlhandle  m;
#endif

} plugin_info_t;


/** Size of the buffer that btun allow for the plugins */
#define PLUGIN_BUFF_SIZE 256


/*

// init plugin & return its information
void bt_plugin_init(plugin_info_t * p);

// destroy plugin
void bt_plugin_destroy(plugin_info_t * p);

// request plugin to encode/decode data, buffer size of in & out is PLUGIN_BUFF_SIZE
ps_t bt_plugin_encode(plugin_info_t * p, char * in, char * out);
ps_t bt_plugin_decode(plugin_info_t * p, char * in, char * out);



*/


#endif
