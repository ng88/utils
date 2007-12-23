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

#ifndef CLIENT_H
#define CLIENT_H


#include "bool.h"
#include "common.h"


/* warning, pass must be writable, this function clear it */
int connect_to_server(char * server, port_t port,
		      char * login, char * pass,
		      char * channel, option_t options,
		      char * cmd, char * args);

void stop_client();

void run_normal(int sockfd, int in, int out);

void run_with_prog(int sockfd, char * p, char * args);

void run_with_prog_on_pty(int sockfd, char * p, char * args);

int writeall(int fd, void * src, size_t s);


#endif
