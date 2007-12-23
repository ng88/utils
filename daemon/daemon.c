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

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void usage(char * app)
{
    fprintf(stderr, "usage: %s command\n", app);
    exit(1);
}

int main(int argc, char ** argv)
{
    char * exename = strrchr(argv[0], '/');
    
    if( !exename )
        exename = argv[0];
     else
         exename++;
         
    if( argc != 2 ) usage(exename);
    
    if( daemon(1, 1) != 0 )
    {
        fprintf(stderr, "%s: daemon() call error\n", exename);
        exit(1);
    }
    
    system(argv[1]);
    
    
    return 0;
}
