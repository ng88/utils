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

typedef enum
{
    M_NORMAL,
    M_EXEC_CMD,
    M_EXEC_CMD_PTY,
} mode_t;


/* warning, pass must be writable, this function clear it */
int connect_to_server(char * server, port_t port,
		      char * login, char * pass,
		      char * channel, option_t options,
		      mode_t m,  char ** cmd_args);

void stop_client();

void run_normal(int sockfd, int in, int out);

void run_cmd(int sockfd, char ** args);

void run_cmd_pty(int sockfd, char ** args);

int writeall(int fd, void * src, size_t s);

char * read_passphrase(char * buff, size_t size);

void flush_std();

#endif
