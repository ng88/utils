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

#include "assert.h"
#include "bool.h"
#include "client.h"
#include "version.h"
#include "protocol.h"
#include "misc.h"
#include "vector.h"

#include "plugin.h"


void usage(int ev)
{
    fputs("usage: " CLIENT_NAME " [options] user@host channel [command arg1 ... argn]\n"
          "  Connect to a btund server on specified host.\n\n"
	  "  Accepted options:\n"
          "   -h                 print this help and quit\n"
          "   -v                 print version and quit\n"
	  "   -t                 use a pseudo terminal for command execution\n"
	  "   -f <file>          read passhrase from 'file' (if 'file' is -, stdout is used)\n"
	  "                      the file must only contains the md5 hash of the user passphrase\n"
	  "   -p <port>          use 'port' instead of the default port (" MXSTR(SERVER_DEFAULT_PORT) ")\n"
#ifdef USE_DL
	  "   -s <name>          try to load the plugin named 'name'. Any number of plugin can be\n"
          "                      loaded, the data stream is processed by each plugin, in load order\n"
          "                      (usual plugins includes compress, xoror, http, tee, ...)\n"
	  "   -o <opt>           pass the option 'opt' to last plugin (loaded with -s)\n"
#endif
	  "   -m                 request to be the master of the channel\n"
	  "   -u                 create an unrestricted channel\n"
	  "   -r                 create a restricted channel (default)\n"
	  "   -a                 create an autoclose channel\n\n"
	  "  Note: -m, -u and -a will ONLY work if you are the first to join the specified channel.\n"
	  "\n"
	  , stderr);
    exit(ev);
}

void print_version(plugin_system_t * e)
{
    puts(CLIENT_NAME " version " CLIENT_VERSION " (r" CLIENT_REVISION ")"
	 " - protocol version " PROTOCOL_VERSION "\n"
	 "   compiled " CLIENT_DATE "\n\n"
	 SC_COPYRIGHT
	);

    if(e)
    {
	size_t i;
	size_t n = plugin_system_count(e);
  
	for(i = 0; i < n; ++i)
	    print_plugin_info(plugin_system_at(e, i), stdout);

	putchar('\n');
    }

    exit(EXIT_SUCCESS);
}

void stop_client_handler(int s)
{
    static int st = 0;
    st = 1 - st;

    if(st == 1)
	stop_client();
}


plugin_system_t * load_plugin(plugin_system_t * e, vector_t * v, char * n)
{
    plugin_system_t * plugins = e;

    if(!plugins)
    {
	plugins = plugin_system_create();
	if(!plugins)
	{
	    fprintf(stderr, 
		    CLIENT_NAME
		    ": unable to load the plugin system: %s\n",
		    plugin_error());
	    exit(EXIT_FAILURE);
	}
    }
    plugin_info_t * plug = plugin_for_namev(n, v);
    vector_clear(v, 0);

    if(!plug)
    {
	fprintf(stderr, 
		CLIENT_NAME
		": unable to load plugin `%s': %s\n",
		n, plugin_error());
	exit(EXIT_FAILURE);
    }

    plugin_system_add(plugins, plug);

    return plugins;
}

int main(int argc, char ** argv)
{

    int optch;

    mode_t mode = M_NORMAL;
    port_t port = SERVER_DEFAULT_PORT;
    FILE * fpass = NULL;
    option_t opts = 0;
    bool show_version = false;

    char * channel;
    char * host;
    char login[USER_MAX_LOGIN_SIZE];
    char pass[MMAX(USER_MAX_PASS_SIZE, MD5_SIZE * 2) + 1];

    char ** cmd_args = NULL;

    plugin_system_t * plugins = NULL;
    vector_t * plugins_args = create_vector(1);
    char * plugins_name = NULL;

    while( (optch = getopt(argc, argv, "o:s:ahvmurtf:p:")) != EOF )
    {
	switch(optch)
	{
	case 'f':
	    if(!strcmp(optarg, "-"))
		fpass = stdin;
	    else
	    {
		if(fpass) fclose(fpass);
		fpass = fopen(optarg, "rb");
		if(!fpass)
		{
		    fprintf(stderr, CLIENT_NAME ": unable to open `%s' for reading\n", optarg);
		    return EXIT_FAILURE;
		}
	    }
	    break;
	case 's':
	    if(plugins_name)
	    {
		plugins = load_plugin(plugins, plugins_args, plugins_name);
		free(plugins_name);
	    }

	    plugins_name = strdup(optarg);
	    break;
	case 'o':
	    if(!plugins_name)
	    {
		fputs(CLIENT_NAME ": -o option can't be used without -s.\n", stderr);
		return EXIT_FAILURE;
	    }
	    vector_add_element(plugins_args, strdup(optarg));
	    break;
	case 'p':
	    port = atoi(optarg);
	    break;
	case 't':
	    mode = M_EXEC_CMD_PTY;
	    break;
	case 'a':
	    opts |= OPT_AUTOCLOSE;
	    break;
	case 'm':
	    opts |= OPT_MASTER;
	    break;
	case 'r':
	    opts &= ~OPT_UNRESTRICTED;
	    break;
	case 'u':
	    opts |= OPT_UNRESTRICTED;
	    break;
	case 'v':
	    show_version = true;
	    break;
	case 'h':
	    usage(EXIT_SUCCESS);
	    break;
	default:
	    usage(EXIT_FAILURE);
	    break;
	}
    }

    if(plugins_name)
    {
	plugins = load_plugin(plugins, plugins_args, plugins_name);
	free(plugins_name);
    }

    if(show_version)
	print_version(plugins);

    /*         LOGIN AND HOST       */
    if(argc - optind < 2)
    {
	fputs(CLIENT_NAME ": argument missing.\n", stderr);
	usage(EXIT_FAILURE);
    }

    host = strchr(argv[optind], '@');
    if(!host)
    {
	fprintf(stderr, CLIENT_NAME ": invalid parameter `%s', login@host expected.\n", 
		argv[optind]);
	return EXIT_FAILURE;
    }

    size_t pos = host - argv[optind];
    host++;
    strncpy(login, argv[optind], min_u((size_t)USER_MAX_LOGIN_SIZE, pos));
    login[pos] = '\0';


    /*            CHANNEL          */
    channel = argv[optind + 1];



    /*       MODE, COMMAND AND ITS ARGS      */
    if(argc - optind > 2)
    {
	if(mode == M_NORMAL)
	    mode = M_EXEC_CMD;

	cmd_args = argv + optind + 2;

    }
    else
    {
	if(mode == M_EXEC_CMD_PTY)
	{
	    fputs(CLIENT_NAME ": you must specify a command with the -t option.\n", stderr);
	    return EXIT_FAILURE;
	}
    }


    /*         PASSPHRASE       */
    if(fpass)
    {
	if(!fgets(pass, MD5_SIZE * 2 + 1, fpass))
	{
	    fputs(CLIENT_NAME ": unable to read passphrase from pass file/stdin!\n", stderr);
	    return EXIT_FAILURE;
	}
	fclose(fpass);
    }
    else
    {
	if(!read_passphrase(pass, USER_MAX_PASS_SIZE))
	{
	    fputs(CLIENT_NAME ": unable to read passphrase!\n", stderr);
	    return EXIT_FAILURE;
	}

	MD5_CTX_ppp m;
	MD5Init_ppp(&m);
	MD5Update_ppp( &m, pass, strlen(pass) );
	MD5Final_ppp(&m);
	
	MD5ToSring(&m, pass);

	memset(&m, '*', sizeof(m));
    }

    flush_std();

    struct sigaction nv, old;
    memset(&nv, 0, sizeof(nv));
    nv.sa_handler = &stop_client_handler;

    sigaction(SIGTERM, &nv, &old);
    sigaction(SIGINT, &nv, &old);
    sigaction(SIGCHLD, &nv, &old);


    if(plugins)
	fprintf(stderr, "WARNING: the plugin feature and the provided plugins are experimental!\n");

    int ret = connect_to_server(host, port, login, pass, channel,
			     opts, mode, cmd_args, plugins);

    if(plugins)
	plugin_system_free(plugins);

    free_vector(plugins_args, 1);

    return ret;
}
