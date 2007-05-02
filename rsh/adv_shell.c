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


#include "adv_shell.h"


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

