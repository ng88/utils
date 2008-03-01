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

#ifndef TCP_CON_H
#define TCP_CON_H

#include "bool.h"
#include "vector.h"

typedef struct
{
    int fd;
    int id;
    bool deleted;
} tcp_connection_t;

tcp_connection_t * create_tcp_infos(int fd, int id);
void free_tcp_infos(tcp_connection_t * e);


#define tcp_infos_count(t) vector_size(t)
#define get_tcp_infos(t, i) \
     ((tcp_connection_t *)vector_get_element_at((t), (i)))
#define add_tcp_infos(t, e) \
     vector_add_element((t), (e))

tcp_connection_t * get_infos_from_id(vector_t * v, int id);

#endif
