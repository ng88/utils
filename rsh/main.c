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

/**
 
   Author: Nicolas GUILLAUME <ng@ngsoft-fr.com>

   rsh is a restricted shell that can execute only few commands.
   It is typically used on a ssh server for user with restricted privilege.

   compile with: make or make ENABLE_READLINE=1 if you want history support

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rsh.h"
#include "conf.h"
#include "adv_shell.h"


int main(int argc, char ** argv)
{
    command_list_t commands;
    char * message;
    char * prompt;
    char * startup;
    FILE * log;

    char * hist_file = conf_get_history_file();

    read_conf(&prompt, &message, &startup, &commands, &log);

    if(log)
	log_write_login(log);

    if(argc == 3 && !strcmp(argv[1], "-c"))
    {
	exec_command(argv[2], commands, log);
    }
    else
    {

	shell_using_history();
	shell_read_history(hist_file);

	puts(message);
	system(startup);

	char * line;

	int cont = 1;
	while(cont &&  (line = shell_line_input(prompt)) )
	{
       
	    if(strlen(line) > 0)
	    {
		if(!exec_command(line, commands, log))
		    cont = 0;

		shell_add_history(line);
	    }
	    
	    free(line);

	}

	shell_write_history(hist_file, MAX_HIST_SIZE);

    }

    if(log)
    {
	log_write_logout(log);
	fclose(log);
    }

    free_command_list(commands);
    free(prompt);
    free(message);
    free(startup);

    return EXIT_SUCCESS;
}
