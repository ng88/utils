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

#include "misc.h"


#ifndef __GNUC__

bool __max_s(signed int a, signed int b) { return a > b ? a : b; }
bool __max_u(unsigned int a, unsigned int b)  { return a > b ? a : b; }
bool __min_s(signed int a, signed int b)  { return a > b ? b : a; }
bool __min_u(unsigned int a, unsigned int b)  { return a > b ? b : a; }


#endif
