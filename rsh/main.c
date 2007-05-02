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

   compile with: gcc -Wall conf.c  main.c  rsh.c -o rsh

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rsh.h"
#include "conf.h"
#include "io.h"


int main(int argc, char ** argv)
{
    command_list_t commands;
    char * message;
    char * prompt;
    FILE * log;

    read_conf(&prompt, &message, &commands, &log);

    if(log)
	log_write_login(log);

    if(argc == 3 && !strcmp(argv[1], "-c"))
    {
	exec_command(argv[2], commands, log);
    }
    else
    {

	puts(message);

	char * line;

	int cont = 1;
	while(cont &&  (line = line_input(prompt)) )
	{
       
	    if(!exec_command(line, commands, log))
		cont = 0;
	    
	    free(line);

	}

    }

    if(log)
    {
	log_write_logout(log);
	fclose(log);
    }

    free_command_list(commands);
    free(prompt);
    free(message);

    return EXIT_SUCCESS;
}
