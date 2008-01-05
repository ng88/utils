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

#ifndef SERVER_H
#define SERVER_H

#include "user.h"
#include "channel.h"
#include "common.h"

#define SERVER_BACKLOG 5

#define SERVER_MAX_CHANNEL 100
#define SERVER_MAX_USER_PER_CHANNEL 10

typedef enum
{
    LE_CONNECTION,
    LE_USER_LOGIN,
    LE_USER_LOGOUT,
} log_event_t;


int start_server(user_pool_t * existing_users, port_t port, FILE * logfile);

/** remove e & return the next entry to delete */
channel_entry_t * remove_user(channel_entry_t * e);

bool process_incoming_data(char * buf, int n, channel_entry_t * e, fd_set * fs);

int get_highest_fd(int fdlist);

size_t index_from_entry(channel_entry_t * e);

void stop_server();

void print_server_status();

void log_write(log_event_t le, channel_entry_t * e);


#endif
