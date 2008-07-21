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


#ifndef DL_H
#define DL_H

#include "version.h"

#ifdef USE_DL
# include <dlfcn.h>
#else

enum { RTLD_NOW = -1 };
static char * dl_err = CLIENT_NAME " was compiled without the plugin support.";

static void dlclose(void * p) { }
static char * dlerror() { return dl_err; }
static void * dlopen(const char *filename, int flag) { return NULL; }
static void * dlsym(void *handle, const char *symbol) { return NULL; }

#endif


#ifndef LIB_EXT
#define LIB_EXT "so"
#endif

#ifndef LIB_PREFIX
#define LIB_PREFIX "libbtp"
#endif

#endif
