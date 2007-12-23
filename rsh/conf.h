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

#ifndef CONF_H
#define CONF_H

#include "rsh.h"

#define CONF_FILE "/etc/rsh.conf"
#define HIST_FILE "~/.rsh_history"

#define MAX_HIST_SIZE 30

#define VERSION "1.3 rev 36"

#define MESSAGE "rhs v" VERSION " (C) by Nicolas GUILLAUME 2007"


char * read_token(char * str, char * token);
char * read_to_quote(char * str, char ** ret);

void read_conf(char ** prompt, char ** message, char ** startup, command_list_t * cmds, FILE ** log);

void syntax_error();

char * conf_get_history_file();


#endif
