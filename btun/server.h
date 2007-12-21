/***************************************************************************
 *   This file is part of the 'utils' projects                             *
 *                                                                         *
 *   'utils' projects                                                      *
 *                                                                         *
 *   Copyright (C) 2006, 2008 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://www.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/                                                                

#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#define SERVER_BACKLOG 5
#define SERVER_DEFAULT_PORT 12358

#define SERVER_MAX_CHANNEL 10
#define SERVER_MAX_USER_PER_CHANNEL 10

typedef uint16_t port_t;


int start_server(port_t port);

void stop_server();

#endif
