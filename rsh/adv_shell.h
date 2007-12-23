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


#ifndef ADV_SHELL_H
#define ADV_SHELL_H

/* get a line from user */
char * shell_line_input(char * prompt);



/* start using history */
void shell_using_history();


/* add 'string' to history */
void shell_add_history(const char *string);


/* load history from file 'filename' */
void shell_read_history(const char *filename);

/* save history to file 'filename'.
 * If max is > 0 then only the last 'max' lines are left */
void shell_write_history(const char *filename, int max);

#endif
