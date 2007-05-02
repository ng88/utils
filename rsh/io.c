#include "io.h"



#ifndef ADV_READLINE

# define _GNU_SOURCE

# include <stdio.h>
# include <string.h>

char * line_input(char * prompt)
{
    char * line = NULL;
    size_t len = 0;

    fputs(prompt, stdout);

    if(getline(&line, &len, stdin) == -1)
	return NULL;

    *(strchr(line, '\n')) = '\0';
    return line;
}

#else

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>


char * line_input(char * prompt)
{using_history();
    return readline(prompt);
}

#endif

