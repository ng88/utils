#ifndef CONF_H
#define CONF_H

#include "rsh.h"

#define CONF_FILE "/etc/rsh.conf"

#define VERSION "1.0"

#define MESSAGE "rhs v" VERSION " (C) by Nicolas GUILLAUME 2007"


char * read_token(char * str, char * token);
char * read_to_quote(char * str, char ** ret);

void read_conf(char ** prompt, char ** message, command_list_t * cmds);

void syntax_error();


#endif
