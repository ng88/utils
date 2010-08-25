/******************************************************************************
 *   btun/btund is a powerful backward tunnel creator                         *
 *                                                                            *
 *   An user guide is available on:                                           *
 *    http://svn.ngsoft-fr.com/trac/utils/wiki/BtunUserGuideEnglish           *
 *                                                                            *
 ******************************************************************************/
 
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
#include <unistd.h>

#include "server.h"
#include "user.h"
#include "version.h"
#include "protocol.h"
#include "misc.h"
#include "bool.h"
#include "assert.h"

#define DEFAULT_USER_FILE "/etc/btund/users"

static char * conf_file;
static user_pool_t * users;


void usage(int ev)
{
    fputs("usage: " SERVER_NAME " [options]\n"
          "  Start the btund server.\n\n"
	  "  Accepted options:\n"
          "   -h                 prints this help and quit\n"
          "   -v                 prints version and quit\n"
	  "   -d                 executes server as a system daemon\n"
	  "   -l <file>          logs information into file\n"
	  "   -n                 disable authentification\n\n"
	  "   -u <file>          specifies the user configuration file\n"
	  "                      (default is " DEFAULT_USER_FILE  ")\n"
	  "   -p <port>          uses 'port' instead of the default port (" MXSTR(SERVER_DEFAULT_PORT) ")\n"
	  "\n"
	  , stderr);
    exit(ev);
}

void print_version()
{
    puts(SERVER_NAME " version " SERVER_VERSION " (r" SERVER_REVISION ")"
	 " - protocol version " PROTOCOL_VERSION "\n"
	 "   compiled " SERVER_DATE "\n\n"
	 SC_COPYRIGHT
	);

    exit(EXIT_SUCCESS);
}

void print_status_handler(int s)
{
    dbg_printf("signal %d\n", s);
    print_server_status();
}

void stop_server_handler(int s)
{
    dbg_printf("signal %d\n", s);
    stop_server();
}

void reload_conf_handler(int s)
{
    dbg_printf("signal %d\n", s);
    dbg_printf("reloading configuration...\n");
    reload_users_from_file(users, conf_file);
}

int main(int argc, char ** argv)
{
    int optch;

    bool exe_daemon = false;
    port_t port = SERVER_DEFAULT_PORT;
    option_t options = 0;

    FILE * fusers = NULL;
    FILE * flog = NULL;
    conf_file = NULL;

    while( (optch = getopt(argc, argv, "l:hnvdp:u:")) != EOF )
    {
	switch(optch)
	{
	case 'u':
	    if(fusers) fclose(fusers);
	    fusers = fopen(optarg, "r");
	    if(!fusers)
	    {
		fprintf(stderr, SERVER_NAME ": unable to open `%s' for reading\n", optarg);
		return EXIT_FAILURE;
	    }
	    else
		conf_file = strdup(optarg);
	    break;
	case 'l':
	    if(flog) fclose(flog);
	    flog = fopen(optarg, "a");
	    if(!flog)
	    {
		fprintf(stderr, SERVER_NAME ": unable to open `%s' for writing\n", optarg);
		return EXIT_FAILURE;
	    }
	    break;
	case 'p':
	    port = atoi(optarg);
	    break;
	case 'd':
	    exe_daemon = true;
	    break;
	case 'n':
	    options |= OPT_NOAUTH;
	    break;
	case 'v':
	    print_version();
	    break;
	case 'h':
	    usage(EXIT_SUCCESS);
	    break;
	default:
	    usage(EXIT_FAILURE);
	    break;
	}
    }

    users = create_user_pool();

    if(!(options & OPT_NOAUTH))
    {
	if(!fusers)
	{
	    fusers = fopen(DEFAULT_USER_FILE, "r");
	    if(!fusers)
	    {
		fputs(SERVER_NAME ": unable to open default user file`" DEFAULT_USER_FILE "'\n",
		      stderr);
		return EXIT_FAILURE;
	    }
	    else
		conf_file = strdup(DEFAULT_USER_FILE);
	}

	read_users_from_file(users, fusers);
	fclose(fusers);
    }

    struct sigaction nv, old;
    memset(&nv, 0, sizeof(nv));
    nv.sa_handler = &stop_server_handler;

    sigaction(SIGTERM, &nv, &old);
    sigaction(SIGINT, &nv, &old);

    nv.sa_handler = &print_status_handler;
    sigaction(SIGUSR1, &nv, &old);
    sigaction(SIGHUP, &nv, &old);

    nv.sa_handler = &reload_conf_handler;
    sigaction(SIGUSR2, &nv, &old);


    if(exe_daemon)
    {
	dbg_printf("daemonisation...\n");
	if(daemon(0, 1) != 0)
	{
	    perror("daemon");
	    return EXIT_FAILURE;
	}
    }

    int r = start_server(users, port, flog, options);

    free_user_pool(users);

    if(conf_file)
	free(conf_file);

    if(flog)
	fclose(flog);

    return r;
}
