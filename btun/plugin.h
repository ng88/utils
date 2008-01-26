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

#ifndef PLUGIN_H
#define PLUGIN_H

#include "plugin_def.h"
#include "assert.h"
#include "vector.h"

typedef struct
{
    vector_t * list;    
} plugin_system_t;


plugin_system_t * plugin_system_create();
void plugin_system_free(plugin_system_t * e);

void plugin_system_add(plugin_system_t * e, plugin_info_t * plug);

size_t plugin_system_encode(plugin_system_t * e, char * in,
			     size_t s, char ** out);

size_t plugin_system_decode(plugin_system_t * e, char * in,
			     size_t s, char ** out);

/** Load plugin named 'name' and initialize it.
    argc && argv are the plugin arguments, if argc == 0 then
    argv can be NULL.
    Return NULL on error.
  */
plugin_info_t * plugin_for_name(char * name, int argc, char ** argv);

/** Same as above execpt it read arguments from a vector.
 */
plugin_info_t * plugin_for_namev(char * name, vector_t * v);

void plugin_free(plugin_info_t * p);

const char * plugin_error();

void print_plugin_info(plugin_info_t * p, FILE * f);


#define plugin_system_count(e) vector_size((e)->list)

#define plugin_system_at(e, i) \
     ((plugin_info_t*)vector_get_element_at((e)->list, (i)))

#endif
