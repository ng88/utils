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

/**

   Author: Nicolas GUILLAUME <ng@ngsoft-fr.com>

   rsh is a restricted shell that can execute only few commands.
   It is typically used on a ssh server for user with restricted privilege.

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "conf.h"
#include "rsh.h"



command_t new_command(char * cmd, char * aliasfor, int hidden)
{
    command_t c = (command_t)malloc(sizeof(*c));

    c->command = cmd;
    c->aliasfor = aliasfor;
    c->hidden = hidden;

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

command_list_t add_head_command_list(command_list_t c, char * cmd, char * aliasfor, int hidden)
{
    node_t n = new_node(c->head, new_command(cmd, aliasfor, hidden));

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


int exec_command(char * str, command_list_t cmds)
{

    if(!strcmp(str, "exit"))
	return 0;

    if(!strcmp(str, "version"))
    {
	puts(MESSAGE);
	return 1;
    }

    if(!strcmp(str, "?"))
    {
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
	if(!strcmp(str, it->cmd->command))
	{
	    system(it->cmd->aliasfor);
	    return 1;
	}

	it = it->next;
    }

    fprintf(stderr, "rsh: %s: command not found, hit ? for a list of command\n", str);

    return 1;

}
