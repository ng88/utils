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

/* this is a trivial program, equivalent to echo 3 > /proc/acpi/sleep
   the only difference is that it can be setuid
*/


#include <stdlib.h>
#include <stdio.h>

#define PROC_SLEEP "/proc/acpi/sleep"

int main()
{
    FILE * f = fopen(PROC_SLEEP, "w");
    if(!f)
    {
	fprintf(stderr, "can open " PROC_SLEEP "!\n");
	return EXIT_FAILURE;
    }

    fputs("3", f);
    fclose(f);

    return EXIT_SUCCESS;
}
