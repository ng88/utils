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

plugin_info_t * plugin_for_name(char * name);
plugin_info_t * plugin_for_index(size_t s);
char * plugin_name(size_t s);
size_t plugin_count();

#define plugin_free(p) \
     do { c_assert((p) && (p)->destructor); \
          (*(p)->destructor)((p)) } while(0)

#define plugin_encode(p, in, out) \
     do { c_assert((p) && (p)->encoder && (in) && (out)); \
          (*p->encoder)((p), (in), (out)); } while(0)

#define plugin_decode(p, in, out) \
     do { c_assert((p) && (p)->encoder && (in) && (out)); \
          (*p->decoder)((p), (in), (out)); } while(0)

#endif
