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

#include "server.h"
#include "user.h"
#include "cryptor.h"

#define DEFAULT_USER_FILE /etc/btund/user.conf


void do_nohing()
{
}

void stop_server_handler(int s)
{
    stop_server();
}

int main(int argc, char ** argv)
{

    struct sigaction nv, old;
    memset(&nv, 0, sizeof(nv));
    nv.sa_handler = &stop_server_handler;

    sigaction(SIGTERM, &nv, &old);
    sigaction(SIGINT, &nv, &old);

    nv.sa_handler = &do_nohing;
    sigaction(SIGUSR1, &nv, &old);
    sigaction(SIGHUP, &nv, &old);

    set_encryptor_mode(C_XOROR);

    user_pool_t * p = create_user_pool();

    FILE * f = fopen("users", "r");
    if(!f)
    {
	fprintf(stderr, "btund: cannot read user file `%s'\n", "users");
	return EXIT_FAILURE;
    }

    read_users_from_file(p, f);

    fclose(f);

    

    int r = start_server(p, SERVER_DEFAULT_PORT);


    free_encryptor();
    free_user_pool(p);

    return r;
}
