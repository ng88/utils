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

#ifndef STATS_H
#define STATS_H

#include <stdio.h>

#include "bool.h"

#ifndef USE_LL
typedef unsigned long long int lsize_t;
#define L_FORMAT "%llu"
#else
typedef unsigned long int lsize_t;
#define L_FORMAT "%lu"
#endif

typedef struct
{
    lsize_t sent;
    lsize_t recv;

    size_t conn_attempt;
    size_t conn_success;

} stats_t;



void init_stats(stats_t * s);

void print_stats(stats_t * s, FILE * f);

#endif
