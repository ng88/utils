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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define DEF_RAND_FILE "/dev/urandom"

typedef enum
{
    OT_NONE, OT_INT, OT_REAL, OT_STRING, OT_DATA
} output_t;


void generate_rand(output_t o, double n, double m);
int generate_rand_int(int n, int m);
double generate_rand_double(double n, double m);

static FILE * rfile = NULL;

void usage(int s)
{
    fputs("usage: nrand [-f file] -i|-r|-s|-d [N] [M]\n\n"
	  "      -f <file>   use file as random source\n"
	  "      -i          generate an integer between N and M\n"
	  "      -r          generate aa real between N and M\n"
	  "      -s          generate a string with a length of N characters\n"
	  "      -d          generate data with a length of N bytes\n"
	  , stderr);
    exit(s);
}


int main(int argc, char ** argv)
{
    int optch;

    output_t type = OT_NONE;
    double n = 0.0;
    double m = 10000.0;

    while( (optch = getopt(argc, argv, "hf:irsd")) != EOF )
    {
	switch(optch)
	{
	case 'f':
	    if(!strcmp(optarg, "-"))
		rfile = stdin;
	    else
	    {
		if(rfile) fclose(rfile);
		rfile = fopen(optarg, "rb");
		if(!rfile)
		{
		    fprintf(stderr, "nrand: unable to open `%s' for reading\n", optarg);
		    return EXIT_FAILURE;
		}
	    }
	    break;
	case 'i': type = OT_INT; break;
	case 'r': type = OT_REAL; break;
	case 's': type = OT_STRING; break;
	case 'd': type = OT_DATA; break;
	case 'h':
	    usage(EXIT_SUCCESS);
	    break;
	default:
	    usage(EXIT_FAILURE);
	    break;
	}
    }

    switch(argc - optind)
    {
    case 2:
	n = atof(argv[optind]);
	m = atof(argv[optind + 1]);
	break;
    case 1:
	m = atof(argv[optind]);
	break;
    }

    if(!rfile)
    {
	rfile = fopen(DEF_RAND_FILE, "rb");
	if(!rfile)
	{
	    fprintf(stderr, "nrand: unable to open `%s' for reading\n", DEF_RAND_FILE);
	    return EXIT_FAILURE;
	}
    }

    if(n > m)
    {
	fprintf(stderr, "nrand: %f is not greater than %f!\n", m, n);
	return EXIT_FAILURE;
    }

    generate_rand(type, n, m);

    fclose(rfile);
    return EXIT_SUCCESS;
}


void generate_rand(output_t o, double n, double m)
{
    int i;
    switch(o)
    {
    case OT_NONE:
	fputs("nrand: type not set, must be one of -i, -r, -s, -d\n", stderr);
	break;
    case OT_INT:
	printf("%d\n", generate_rand_int((int)n , (int)m));
	break;
    case OT_REAL:
	printf("%f\n", generate_rand_double(n , m));
	break;
    case OT_STRING:
	for(i = 0; i < (int)m; ++i)
	    putchar((char)generate_rand_int(32, 126));
	putchar('\n');
	break;
    case OT_DATA:
	for(i = 0; i < (int)m; ++i)
	    putchar((char)fgetc(rfile));
	break;
    }
}

int generate_rand_int(int n, int m)
{
    unsigned int r;
    fread(&r, sizeof(r), 1, rfile);
    srand(r);
    return n + (int) ((double)rand() * (m - n + 1) / (RAND_MAX + 1.0));
}

double generate_rand_double(double n, double m)
{
    unsigned int r;
    fread(&r, sizeof(r), 1, rfile);
    srand(r);
    return n + ((double)rand() / (RAND_MAX + 1.0)) * (m - n);
}
