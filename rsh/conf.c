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


#define _GNU_SOURCE
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "conf.h"



char * read_token(char * str, char * token)
{
    while(*token)
    {
	if(*token != *str)
	    return NULL;

	token++;
	str++;

	if(!*str)
	    return NULL;
    }

    return str;
}

char * read_to_quote(char * str, char ** ret)
{
    const int N = 1024;
    *ret = (char*)malloc(N);

    int count = 0;

    while(*str && count < N - 2)
    {
	if(*str == '\'')
	{
	    str++;
	    break;
	}
	else
	    (*ret)[count] = *str;

	str++;
	count++;
    }

    (*ret)[count] = '\0';
    return str;
}

void syntax_error()
{
    fputs("syntax error in configuration file!\n", stderr);
    exit(EXIT_FAILURE);
}

void read_conf(char ** prompt, char ** message, command_list_t * cmds, FILE ** log)
{

    char * username = getpwuid(getuid())->pw_name;

    *cmds = new_command_list();
    *message = NULL;
    *prompt = NULL;
    *log  = NULL;

    /* read configuration */
    FILE * f = fopen(CONF_FILE, "r");

    if(!f)
    {
	fputs("unable to open configuration file " CONF_FILE "!\n", stderr);
	exit(EXIT_FAILURE);
    }
    else
    {
	char * line = NULL;
	size_t len = 0;
	size_t read;
        char * pos;

	while ((read = getline(&line, &len, f)) != -1)
	{
	    if(read > 1 && line[0] != '#')
	    {
		pos = read_token(line, "for ");

		if(!pos) syntax_error();

		if(*pos == '*')
		    pos++;
		else
		    pos = read_token(pos, username);

		if(pos) /* conf for current user */
		{
		    char * test = read_token(pos, " message is '");
		    if(test) /* message configuration */
		    {
			if(*message)
			    free(*message);

			read_to_quote(test, message);

		    }


		    test = read_token(pos, " prompt is '");
		    if(test) /* prompt configuration */
		    {
			if(*prompt)
			    free(*prompt);

			read_to_quote(test, prompt);

		    }



		    test = read_token(pos, " allow hidden '");
		    if(test) /* allow hidden command */
		    {
			char * cmd, *alias;

			test = read_to_quote(test, &alias);
			test = read_token(test, " as '");

			if(!test) syntax_error();

			test = read_to_quote(test, &cmd);

			add_head_command_list(*cmds, cmd, alias, 1);
		    }


		    test = read_token(pos, " allow '");
		    if(test) /* allow command */
		    {
			char * cmd, *alias;

			test = read_to_quote(test, &alias);
			test = read_token(test, " as '");

			if(!test) syntax_error();

			test = read_to_quote(test, &cmd);

			add_head_command_list(*cmds, cmd, alias, 0);
		    }

		    test = read_token(pos, " log into '");
		    if(test) /* allow command */
		    {
			char * logpath;

			test = read_to_quote(test, &logpath);

			if(*log)
			    fclose(*log);

			*log = fopen(logpath, "a");

			if(!*log)
			    fprintf(stderr, "warning: unable to open log file `%s' for writing!\n", logpath);
		    }

		}
	    }


	    if (line)
	    {
		free(line);
		line = NULL;
	    }

	}

	if (line)
	    free(line);

	fclose(f);

	if(!*message) *message = strdup("Welcome to rsh!");

	if(!*prompt) *prompt = strdup("rhs> ");

    }

}
