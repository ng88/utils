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


#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "xoror.h"


/*
   Author: Nicolas GUILLAUME

  Xoror is a simple xor cryptor.

  Warning: This may NOT resist anybody.
           Do NOT use it for critical data.

     Compile with:  gcc -Wall xoror.c main.c -o xoror

*/


/* -h -p blabla -k 456 -s salut|-i file1 -o file2*/
void usage(const char * pname, int ev)
{
    fprintf(stderr, "usage: %s [-h] -p passphrase [-k key] [-s string] [-i file_in] [-o file_out]\n\n"
	            "       -h                show this help\n"
	            "       -p passphrase     passphrase used to crypt data\n"
		    "       -k key            key used with passphrase to crypt data\n"
		    "       -s string         crypt string, stdout is used as output\n"
		    "       -i file           input file, cannot be used with -s\n"
		    "                         if not specified, stdin is used\n"
		    "       -o file           output file, if not specified, stdout\n"
		    "                         is used\n"
		    "\n"
	            "       Examples :\n\n"
	            "          Encrypt 1.tgz to 1.tgz.crypt\n"
		    "          %s -p \"this is a test\" -i 1.tgz -o 1.tgz.crypt\n\n"
	            "          Decrypt 1.tgz.crypt back to 1.tgz\n"
		    "          %s -p \"this is a test\" -o 1.tgz -i 1.tgz.crypt\n\n"
	            "          Encrypt 'salut'\n"
		    "          %s -p \"this is a test\" -s salut\n"
		    , pname, pname, pname, pname);
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

    int key = 0;
    char * pass = NULL;
    char * string = NULL;
    FILE * in = NULL;
    FILE * out = NULL;

    while( (optch = getopt(argc, argv, "p:k:s:hi:o:")) != -1 )
    {
	switch(optch)
	{
	case 'i':
	    in = fopen(optarg, "rb");
	    if(!in)
	    {
		fprintf(stderr, "%s: unable to open input file `%s'\n", pname, optarg);
		return EXIT_FAILURE;
	    }
	    break;
	case 'o':
	    out = fopen(optarg, "wb");
	    if(!out)
	    {
		fprintf(stderr, "%s: unable to open output file `%s'\n", pname, optarg);
		return EXIT_FAILURE;
	    }
	    break;
	case 'p':
	    pass = optarg;
	    break;
	case 'k':
	    key = atoi(optarg);
	    break;
	case 's':
	    string = optarg;
	    break;
	case 'h':
	    usage(pname, EXIT_SUCCESS);
	    break;
	default:
	    usage(pname, EXIT_FAILURE);
	    break;
	}
    }

    if(argc - optind)
	usage(pname, EXIT_FAILURE);

    if(in && string)
	usage(pname, EXIT_FAILURE);

    if( !pass || strlen(pass) <= 2)
	usage(pname, EXIT_FAILURE);

    if(!key)
	key = pass[0] * pass[1];

    cryptor * c = cryptor_new(pass, key);

    if(!out) out = stdout;

    if(string) /* working with string */
    {
	int n = strlen(string);
	char * outs = (char*)malloc(n + 1);

	encrypt_string(string, outs, c);
	
	int i;
	for(i = 0; i < n; ++i)
	    fputc(outs[i], out);

	free(outs);
    }
    else /* working with files */
    {
	if(!in) in = stdin;
 
	encrypt_file(in, out, c);

	fclose(in);
    }

    fclose(out);

    cryptor_free(c);

    return EXIT_SUCCESS;

}

