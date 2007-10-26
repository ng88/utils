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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sudoku.h"

void usage(const char * pname, int ev)
{
    fprintf(stderr, "usage: %s [option]\n"
	            "  Accepted options:\n"
	            "   -p                plain text output (default)\n"
	            "   -w                html output\n"
                    "   -f file           input file, stdin is used if not specified\n"
                    "   -q                display only the solution\n"
		    , pname);
    exit(ev);
}

int main(int argc, char ** argv)
{
    char * pname = strrchr(argv[0], '/');
    if( !pname )
        pname = argv[0];
    else
	pname++;

    int optch;

    FILE * f = stdin;
    void (*print_format)(grid_t*) = &ss_print_grid_text;
    int quiet = 0;

    while( (optch = getopt(argc, argv, "wf:phq")) != -1 )
    {
	switch(optch)
	{
	case 'w':
	    print_format = &ss_print_grid_html;
	    break;
	case 'p':
	    print_format = &ss_print_grid_text;
	    break;
	case 'f':
	     	f = fopen(optarg, "r");
		if(!f)
		{
		    fprintf(stderr, "Unable to read `%s'!\n", optarg);
		    return EXIT_FAILURE;
		}
	    break;
	case 'q':
	    quiet = 1;
	    break;
	default:
	    usage(pname, EXIT_FAILURE);
	    break;
	}
    }


    grid_t g;

    if(!ss_read_grid_from_file(f, &g))
    {
	fputs("Unable to load the grid correctly!\n", stderr);

	fclose(f);
	return EXIT_FAILURE;
    }
    fclose(f);

    if(!quiet)
	(*print_format)(&g);

    if(ss_check_grid(&g))
    {
	
	if(ss_solve_grid(&g))
	{
	    puts("Solution:");
	    (*print_format)(&g);
	}
	else
	    puts("Unable to solve this grid!");
    }
    else
	puts("Invalid grid!");
   
    return EXIT_SUCCESS;

}
