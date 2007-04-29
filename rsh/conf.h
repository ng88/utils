/***************************************************************************
 *   This file is part of the 'utils' projects                             *
 *                                                                         *
 *   'utils' projects                                                      *
 *                                                                         *
 *   Copyright (C) 2006, 2007 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://www.ngsoft-fr.com/                                             *
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

#define VERSION "1.2 rev 14"

#define MESSAGE "rhs v" VERSION " (C) by Nicolas GUILLAUME 2007"


char * read_token(char * str, char * token);
char * read_to_quote(char * str, char ** ret);

void read_conf(char ** prompt, char ** message, command_list_t * cmds, FILE ** log);

void syntax_error();


#endif
