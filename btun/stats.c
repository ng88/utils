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


#include "stats.h"
#include "assert.h"


void init_stats(stats_t * s)
{
    c_assert(s);
    s->recv = s->sent = (lsize_t)0;
    s->conn_attempt = s->conn_success = (size_t)0;
}


void print_stats(stats_t * s, FILE * f)
{
    c_assert(s && f);

    fprintf(f, "Data sent: " L_FORMAT "b\n"
	       "Data received: " L_FORMAT "b\n"
	       "Connection attempts: %u\n"
  	       "Connection successful: %u\n",
	    s->sent, s->recv, s->conn_attempt, 
	    s->conn_success);
    

}
