/**

   Author: Nicolas GUILLAUME <ng@ngsoft-fr.com>

   rsh is a restricted shell that can execute only few commands.
   It is typically used on a ssh server for user with restricted privilege.

*/

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rsh.h"
#include "conf.h"



int main(int argc, char ** argv)
{
    command_list_t commands;
    char * message;
    char * prompt;

    read_conf(&prompt, &message, &commands);

    if(argc == 3 && !strcmp(argv[1], "-c"))
    {
	exec_command(argv[2], commands);
    }
    else
    {

	puts(message);

	char * line = NULL;
	size_t len = 0;
	size_t read;

	fputs(prompt, stdout);
	while ((read = getline(&line, &len, stdin)) != -1)
	{
       
	    *(strchr(line, '\n')) = '\0';
	

	    if(read > 1 && !exec_command(line, commands))
		break;

	    fputs(prompt, stdout);

            if (line)
            {
                free(line);
                line = NULL;
            }


	}

	if (line)
	    free(line);

    }

    free_command_list(commands);
    free(prompt);
    free(message);

    return EXIT_SUCCESS;
}
