/***************************************************************************
 * Tar Stream Format                                                       *
 * Tar-like archiving that can be easily streamed over HTTP                *
 ***************************************************************************/
 
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "tsf.h"


void usage(const char * p)
{
    fprintf(stderr,
	    "usages: %s c destination.tsf files\n"
            "              Create an archives with files\n"
            "              c can be followed by the compression level (c0, c5, ... c9)\n"
	    "        %s x destination.tsf [folder]\n",
	    p, p);
    exit(EXIT_FAILURE);
}

void extract_archive(const char * archive, const char * dir, tsf_options_t * opt)
{
    tsf_archive_t * a = tsf_open_archive(archive, TSF_READ, opt);
    if(!a)
    {
	fprintf(stderr, "cannot open %s!\n", archive);
	return;
    }

    //TODO
    (void)dir;

    tsf_close_archive(a);
}

void create_archive(const char * archive, char * files[], int file_count, tsf_options_t * opt)
{

    tsf_archive_t * a = tsf_open_archive(archive, TSF_WRITE, opt);

    if(!a)
    {
	fprintf(stderr, "cannot open %s!\n", archive);
	return;
    }

    tsf_begin_entries(a);

    int i;
    for(i = 0; i < file_count; i++)
    {
	if(opt->verbose_level > 0)
	    printf("Appending %s...\n", files[i]);
	if(tsf_append_tree_entries(a, files[i]) < 0)
	{
	    perror("appending dir tree");
	    break;
	}
    }

    tsf_end_entries(a);    

    tsf_close_archive(a);
}

void arg2options(const char * arg, tsf_options_t * opt)
{
    int n = strlen(arg);
    int i;
    opt->cpr_level = 5;
    opt->tree_flags = 0;
    opt->verbose_level = 0;
    for(i = 0; i < n; i++)
    {
	switch(arg[i])
	{
	case '0': case '1':  case '2':
	case '3': case '4':  case '5':
	case '6': case '7':  case '8':
	case '9':
	    opt->cpr_level = arg[i] - '0';
	    break;
	case 'v':
	case 'V':
	    opt->verbose_level++;
	    break;
	}
    }
}

int main(int argc, char * argv[])
{
    if(argc < 3)
	usage(argv[0]);

    const char * archive = argv[2];
    tsf_options_t opt;

    arg2options(argv[1], &opt);

    switch(argv[1][0])
    {
    case 'x':
    case 'X':	
	extract_archive(archive, argc < 4 ? "." : argv[3], &opt);
	break;

    case 'c':
    case 'C':
	if(argc < 4)
	    usage(argv[0]);

	create_archive(archive, &argv[3], argc - 3, &opt);
	break;
    default:
	usage(argv[0]);
    }

    return EXIT_SUCCESS;
}
