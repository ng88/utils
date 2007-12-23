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

/**

   Author: Nicolas GUILLAUME <ng@ngsoft-fr.com>

   rsh is a restricted shell that can execute only few commands.
   It is typically used on a ssh server for user with restricted privilege.

*/
#define _GNU_SOURCE
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "conf.h"
#include "rsh.h"



command_t new_command(char * cmd, char * aliasfor, int hidden, int allow_arg)
{
    command_t c = (command_t)malloc(sizeof(*c));

    c->command = cmd;
    c->aliasfor = aliasfor;
    c->hidden = hidden;
    c->allow_arg = allow_arg;
    return c;
}


void free_command(command_t c)
{
    free(c->command);
    free(c->aliasfor);
    free(c);
}

command_list_t new_command_list()
{
    command_list_t c = (command_list_t)malloc(sizeof(*c));

    c->head = NULL;
    c->size = 0;

    return c;
}

void free_command_list(command_list_t c)
{
    if(c->head)
	free_node(c->head);

    free(c);
}

command_list_t add_head_command_list(command_list_t c, char * cmd, char * aliasfor, int hidden, int allow_arg)
{
    node_t n = new_node(c->head, new_command(cmd, aliasfor, hidden, allow_arg));

    c->head = n;
    c->size++;

    return c;
}

node_t new_node(node_t next, command_t cmd)
{
    node_t n = (node_t)malloc(sizeof(*n));
    n->next = next;
    n->cmd = cmd;
    return n;
}

void free_node(node_t n)
{
    if(n->next)
	free_node(n->next);

    free_command(n->cmd);
    free(n);
}


int exec_command(char * str, command_list_t cmds, FILE * log)
{
    int n = strlen(str);

    if(log)
	log_write_execcmd(log, str);

    if(!strcmp(str, "exit"))
    {
	if(log)
	    log_write_rescmd(log, "rsh.builtin.exit");

	return 0;
    }

    if(!strcmp(str, "version"))
    {
	if(log)
	    log_write_rescmd(log, "rsh.builtin.version");

	puts(MESSAGE);
	return 1;
    }

    if(!strcmp(str, "?"))
    {
	if(log)
	    log_write_rescmd(log, "rsh.builtin.help");

	puts("Available commands:");
	node_t it = cmds->head;

	while(it)
	{
	    if(!it->cmd->hidden)
		puts(it->cmd->command);
	    it = it->next;
	}
	return 1;
    }

    node_t it = cmds->head;

    while(it)
    {

	if(strcmp_arg(str, it->cmd->command, it->cmd->allow_arg))
	{

	    char * exec;

	    if(it->cmd->allow_arg)
	    {
		exec = (char*)malloc(strlen(it->cmd->aliasfor) + n);
		strcpy(exec, it->cmd->aliasfor);

		char * pos = strchr(str, ' ');

		if(pos)
		    strcat(exec, pos);
	    }
	    else
		exec = it->cmd->aliasfor;

	    if(log)
		log_write_rescmd(log, exec);

	    system(exec);

	    if(it->cmd->allow_arg)
		free(exec);

	    return 1;

	}

	it = it->next;
    }

    if(log)
	log_write_rescmd(log, "rsh.builtin.command_not_found");

    fprintf(stderr, "rsh: %s: command not found, hit ? for a list of command\n", str);

    return 1;

}

int strcmp_arg(char * user, char * cmd, int allow_arg)
{
    while( *user && *cmd )
    {
	if(*user != *cmd)
	    return 0;
	
	user++;
	cmd++;

	if(allow_arg && *user == ' ')
	    break;
    }

    return *cmd == '\0' && (*user == ' ' || *user == '\0');
}

void log_write_rescmd(FILE * log, char * cmd)
{
    log_write_linehead(log);
    fprintf(log, "command resulted in `%s'\n", cmd);
    fflush(log);
}

void log_write_execcmd(FILE * log, char * cmd)
{
    log_write_linehead(log);
    fprintf(log, "executed command `%s'\n", cmd);
    fflush(log);
}

void log_write_str(FILE * log, char * str)
{
    log_write_linehead(log);
    fputs(str, log);
    fflush(log);
}

void log_write_linehead(FILE * log)
{
    uid_t uid = getuid();

    time_t t = time(NULL);
    struct tm * date =  localtime(&t);

    fprintf(log, "[%02d:%02d:%02d %02d/%02d/%4d] user %d (%s) ",
	    date->tm_hour,
	    date->tm_min,
	    date->tm_sec,
	    date->tm_mday,
	    date->tm_mon + 1,
	    date->tm_year + 1900,
	    uid,
	    getpwuid(uid)->pw_name);
}


