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

#ifndef BOOL_H
#define BOOL_H

#ifdef FALSE
# undef FALSE
#endif

#ifdef TRUE
# undef TRUE
#endif

#ifdef false
# undef false
#endif

#ifdef true
# undef true
#endif

#ifdef bool
# undef bool
#endif

#define FALSE false
#define TRUE  true
#define true  (!false)
#define false 0
#define bool  __bool__


typedef unsigned char __bool__;

#endif

