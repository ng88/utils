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

#ifndef RSH_H
#define RSH_H


typedef struct
{
    /* real command */
    char * command;

    /* alias */
    char * aliasfor;

     /* shown in '?' list ? */
    int hidden;

    /* arguments allowed ? */
    int allow_arg;

}* command_t;


typedef struct s_node
{
    struct s_node * next;
    command_t cmd;
}* node_t;


typedef struct
{
    node_t head;
    size_t size;
}* command_list_t;


command_t new_command(char * cmd, char * aliasfor, int hidden, int allow_arg);
void free_command(command_t c);



command_list_t new_command_list();
void free_command_list(command_list_t c);
command_list_t add_head_command_list(command_list_t c, char * cmd, char * aliasfor, int hidden, int allow_arg);



node_t new_node(node_t next, command_t cmd);
void free_node(node_t n);


int exec_command(char * str, command_list_t cmds, FILE * log);

/* return non zero is user is the command cmd */
int strcmp_arg(char * user, char * cmd, int allow_arg);

#define log_write_login(log) log_write_str((log), "logged in\n")
#define log_write_logout(log) log_write_str((log), "logged out\n")

void log_write_execcmd(FILE * log, char * cmd);
void log_write_rescmd(FILE * log, char * cmd);
void log_write_str(FILE * log, char * str);
void log_write_linehead(FILE * log);

#endif
//tester avec ~ mettre a jour le conf exemple
