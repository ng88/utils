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

#include "plugin.h"
#include <string.h>

#include <dlfcn.h>

#define LIB_EXT "so"

plugin_system_t * plugin_system_create()
{
    plugin_system_t * r = (plugin_system_t *)malloc(sizeof(plugin_system_t));
    c_assert2(r, "malloc failed");

    r->list = create_vector(2);

    return r;
}

void plugin_system_free(plugin_system_t * e)
{
    c_assert(e);

    size_t i;
    size_t n = plugin_system_count(e);

    for(i = 0; i < n; ++i)
    {
	plugin_info_t * p = plugin_system_at(e, i);

        dbg_printf("freeing plugin `%s'...\n", p->name);
	plugin_free(p);
    }

    free_vector(e->list, 0);
    free(e);

}
void plugin_free(plugin_info_t * p)
{
    c_assert(p && p->destructor);

    (*p->destructor)(p);

    if(p->argv)
    {
	int i;
	for(i = 0; i < p->argc; ++i)
	    free(p->argv[i]);

	free(p->argv);
    }

    dlclose(p->module);
    free(p);
}

void plugin_system_add(plugin_system_t * e, plugin_info_t * p)
{
    c_assert(p && e);

    dbg_printf("inserting plugin `%s' version %u...\n", p->name, p->version);

    vector_add_element(e->list, p);
}


size_t plugin_system_encode(plugin_system_t * e, char * in,
			     size_t s, char ** out)
{
    c_assert(e && in);

    size_t i;
    size_t n = plugin_system_count(e);

    char * buffer = in;
    size_t buffer_size = s;
  
    for(i = 0; i < n; ++i)
    {
	plugin_info_t * p = plugin_system_at(e, i);
	char * plugin_buffer;
	
	size_t bs = (*p->encoder)(p, buffer, buffer_size, &plugin_buffer);

	if(bs == BT_ERROR)
	{
	    dbg_printf("plugin error\n");
	    return BT_ERROR;
	}

	buffer = plugin_buffer;
	buffer_size = bs;
    }

    *out = buffer;
    return buffer_size;
}

size_t plugin_system_decode(plugin_system_t * e, char * in,
			     size_t s, char ** out)
{
    c_assert(e && in);

    size_t i;
    size_t n = plugin_system_count(e);

    char * buffer = in;
    size_t buffer_size = s;
  
    for(i = n; i > 0; --i)
    {
	plugin_info_t * p = plugin_system_at(e, i - 1);
	char * plugin_buffer;
	
	size_t bs = (*p->decoder)(p, buffer, buffer_size, &plugin_buffer);

	if(bs == BT_ERROR)
	    return BT_ERROR;

	buffer = plugin_buffer;
	buffer_size = bs;
    }

    *out = buffer;
    return buffer_size;
}


plugin_info_t * plugin_for_name(char * name, int argc, char ** argv)
{
    c_assert(name);
    c_assert( argc == 0 || (argc && argv) );

    plugin_info_t * r = (plugin_info_t *)malloc(sizeof(plugin_info_t));
    c_assert2(r, "malloc failed");

    r->name = NULL;
    r->desc = NULL;
    r->author = NULL;
    r->version = 0;
    r->data = NULL;
    r->buffer = NULL;
    r->buffer_size = 0;
    r->argc = argc;
    r->argv = argv;

    char * lname = NULL;
    fn_plug_init_t init = NULL;

    size_t len = strlen(name);

    if(!strchr(name, '.'))
    {
	lname = (char*)malloc(len + sizeof(LIB_EXT) + 1);
	c_assert2(lname, "malloc failed");
	sprintf(lname, "%s." LIB_EXT, name);
    }

    if( (r->module = dlopen(lname ? lname : name, RTLD_NOW)) )
    {
	init = (fn_plug_init_t)dlsym(r->module, "bt_plugin_init");
	r->destructor = (fn_plug_free_t)dlsym(r->module, "bt_plugin_destroy");

	r->encoder = (fn_plug_inout_t)dlsym(r->module, "bt_plugin_encode");

	r->decoder = (fn_plug_inout_t)dlsym(r->module, "bt_plugin_decode");
    }

    if(lname)
	free(lname);

    if(init)
    {
	if( !(*init)(r) )
          /* plugin init failed */
	    return NULL;
    }

    if(!r->module || !r->name || !r->destructor
       || !r->encoder || !r->decoder || !init)
    {
	dbg_printf("plugin `%s' cannot be loaded .\n", name);
	free(r);
	return NULL;
    }

    dbg_printf("plugin `%s' loaded .\n", name);

    return r;
}

plugin_info_t * plugin_for_namev(char * name, vector_t * v)
{
    c_assert(v && name);

    int argc = (int)vector_size(v);
    char ** argv = NULL;

    if(argc)
    {
	argv = (char**)malloc(sizeof(char*) * argc);
	c_assert2(argv, "malloc failed");

	int i;
	for(i = 0; i < argc; ++i)
	    argv[i] = vector_get_element_at(v, (size_t)i);
    }

    return plugin_for_name(name, argc, argv);
}

const char * plugin_error()
{
    return dlerror();
}


void print_plugin_info(plugin_info_t * p, FILE * f)
{
    c_assert(p);

    fprintf(f, "Plugin %s v%u: %s (%s)\n",
	    (p->name ? p->name : "???"),
	    p->version,
	    (p->desc ? p->desc : "???"),
	    (p->author ? p->author : "???")   
	);

}

