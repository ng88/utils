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


/*
  NOTE : en fait, faire ca avec dynamique seulement, ca sera plus simple



*/


static char * static_plug_name = 
                     {
#ifdef STP_COMPRESS		      
			 "compress",
#endif
#ifdef STP_XOROR
			 "xoror",
#endif
#ifdef STP_SSL
			 "aes128",
			 "aes256",
#endif
		     };

#define PLUG_STATIC_COUNT (sizeof(static_plug_name) / sizeof(char*))


plugin_info_t * plugin_for_index(size_t s)
{
    c_assert(s < plugin_count());

    /* static */
    if(s < PLUG_STATIC_COUNT)
    {
	size_t i = 0;
#ifdef STP_COMPRESS
#endif

	switch(s)
	{
	case 0: return compress_plugin_init()
	case 1: return xoror_plugin_init();
	case 2: return ssl_plugin_init(AES_128);
	case 3: return ssl_plugin_init(AES_256);
	}
    }
    else
    {
	/* dynamic */
    }

    return NULL;

}




plugin_info_t * plugin_for_name(char * name)
{

    c_assert(name);

    size_t i;
    for(i = 0; i < plugin_count(); ++i)
	if(!strcmp(name, plugin_name(i))
	   return plugin_for_index(i);
    

    return NULL;
}


size_t plugin_count()
{
    return PLUG_STATIC_COUNT;
}

char * plugin_name(size_t s)
{
    c_assert(s < plugin_count());
    return static_plug_name[s];
}


