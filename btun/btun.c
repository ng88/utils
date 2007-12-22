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


#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "assert.h"

#include "bool.h"
#include "client.h"

int main(int argc, char ** argv)
{

    char * login;
    char  pass[USER_MAX_PASS_SIZE];
    char * channel;
    char * server;
    option_t options = 0;
    port_t port = SERVER_DEFAULT_PORT;

    /*      Temporaire     */
    c_assert2(argc >= 5, "# d'argument incorrect");
    server = argv[1];
    login = argv[2];
    strncpy(pass, argv[3], USER_MAX_PASS_SIZE);
    channel = argv[4];

    if(argc >= 6 && argv[5][0] == 'M')
	options = options | OPT_MASTER;

    /*      Temporaire     */


    return connect_to_server(server, port, login, pass, channel, options);

}
