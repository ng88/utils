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


#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "assert.h"

#include "bool.h"
#include "client.h"
#include "version.h"


void stop_client_handler(int s)
{
    stop_client();
}

int main(int argc, char ** argv)
{
    struct sigaction nv, old;
    memset(&nv, 0, sizeof(nv));
    nv.sa_handler = &stop_client_handler;

    sigaction(SIGTERM, &nv, &old);
    sigaction(SIGINT, &nv, &old);


    char * login;
    char  pass[MD5_SIZE * 2];
    char * channel;
    char * server;
    option_t options = 0;
    port_t port = SERVER_DEFAULT_PORT;
    char * cmd = NULL;

    /*      Temporaire     */
    c_assert2(argc >= 5, "# d'argument incorrect");
    server = argv[1];
    login = argv[2];
    MD5_CTX_ppp m;
    MD5Init_ppp(&m);
    MD5Update_ppp(&m, argv[3], strlen(argv[3]));
    MD5Final_ppp(&m);
    MD5ToSring(&m, pass);
    channel = argv[4];

    if(argc >= 6 && argv[5][0] == 'M')
	options = options | OPT_MASTER;

    if(argc >= 7)
	cmd = argv[6];

    /*      Temporaire     */


    //return connect_to_server(server, port, login, pass, channel, options, cmd, NULL);
    puts("vers=" CLIENT_VERSION " rev=" CLIENT_REVISION " date=" CLIENT_DATE);

}
