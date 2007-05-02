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

char * shell_line_input(char * prompt)
{
    char * line = NULL;
    size_t len = 0;

    fputs(prompt, stdout);

    if(getline(&line, &len, stdin) == -1)
	return NULL;

    *(strchr(line, '\n')) = '\0';
    return line;
}

void shell_using_history() {}

void shell_add_history(const char *string) {}

void shell_read_history(const char *filename) {}
void shell_write_history(const char *filename, int max) {}

#else

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>


char * shell_line_input(char * prompt)
{
    return readline(prompt);
}

void shell_using_history()
{
    using_history();
}

void shell_add_history(const char *string)
{
    add_history(string);
}

void shell_read_history(const char *filename)
{
    read_history(filename);
}

void shell_write_history(const char *filename, int max)
{
    write_history(filename);

    if(max > 0)
	history_truncate_file(filename, max);
}

#endif

