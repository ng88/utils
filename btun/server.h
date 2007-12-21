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

#include "user.h"
#include "channel.h"
#include "protocol.h"

#define SERVER_BACKLOG 5

#define SERVER_MAX_CHANNEL 10
#define SERVER_MAX_USER_PER_CHANNEL 10


typedef uint16_t port_t;


int start_server(user_pool_t * existing_users, port_t port);

void send_challenge(int fd);

bool send_data(channel_entry_t * e, char * data, size_t len);
bool recv_data(channel_entry_t * e, char * data, size_t len);

void process_incoming_data(char * buf, int n, user_pool_t * existing_users, 
			   vector_t * u, channel_entry_t * e, fd_set * fs);


int get_highest_fd(vector_t * u, int fdlist);


void stop_server();

#endif
