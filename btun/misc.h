/******************************************************************************
 *   btun/btund is a powerful backward tunnel creator                         *
 *                                                                            *
 *   An user guide is available on:                                           *
 *    http://abygaelle.guillaume-fr.net/trac/utils/wiki/BtunUserGuideEnglish  *
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

#ifndef MISC_H
#define MISC_H

#include "bool.h"

#define MXSTR(s) MSTR(s)
#define MSTR(s) #s

#ifdef __GNUC__

#define __max(a,b) \
       ({ typeof (a) _a = (a); \
           typeof (b) _b = (b); \
         _a > _b ? _a : _b; })


#define __min(a,b) \
       ({ typeof (a) _a = (a); \
           typeof (b) _b = (b); \
         _a > _b ? _b : _a; })

#define max_s(a, b) __max(a, b)
#define max_u(a, b) __max(a, b)

#define min_s(a, b) __min(a, b)
#define min_u(a, b) __min(a, b)

#else

bool __max_s(signed int a, signed int b);
bool __max_u(unsigned int a, unsigned int b);
bool __min_s(signed int a, signed int b);
bool __min_u(unsigned int a, unsigned int b);

#define max_s(a, b) __max_s(a, b)
#define max_u(a, b) __max_u(a, b)

#define min_s(a, b) __min_s(a, b)
#define min_u(a, b) __min_u(a, b)

#endif


#define MMAX(a, b) ((a) > (b) ? (a) : (b))
#define MMIN(a, b) ((a) > (b) ? (b) : (a))

#endif
