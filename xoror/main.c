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
#include <string.h>
#include <unistd.h>

#include "xoror.h"


/*
   Author: Nicolas GUILLAUME

  Xoror is a simple xor cryptor.

  Warning: This may NOT resist anybody.
           Do NOT use it for critical data.

     Compile with:  make

*/

#define PROG_VERSION_MAJOR 0
#define PROG_VERSION_MINOR 9

#define MAX_PASS 4096

void version(const char * pname)
{
    printf("%s v%d.%d, cryptor v%d.%d\n\n"
	   "Copyright (C) 2006, 2008 by GUILLAUME Nicolas\n"
	   "ng@ngsoft-fr.com\n\n", pname,
	   PROG_VERSION_MAJOR, PROG_VERSION_MINOR,
	   LAST_ALGO_VERSION_MAJOR, LAST_ALGO_VERSION_MINOR);

    exit(EXIT_SUCCESS);
}

void usage(const char * pname, int ev)
{
    fprintf(stderr, "usage: %s [-h] [-v] [-p passphrase | -r | -f passfile] [-k key]\n"
	            "          [-s string | -i file_in] [-o file_out]\n\n"
	            "       -h                show this help\n"
	            "       -v                show version\n"
	            "       -p passphrase     passphrase used to crypt data\n"
	            "       -f file           read passphrase from file (or from stdin if - is\n"
                    "                         specified), max passphrase size is %d bytes\n"
	            "       -r                deprecated, equivalent of -f -\n"
		    "       -k key            key used with passphrase to crypt data\n"
		    "       -s string         crypt string, stdout is used as output\n"
		    "       -i file           input file, cannot be used with -s\n"
		    "                         if not specified, stdin is used\n"
		    "       -o file           output file, if not specified, stdout\n"
		    "                         is used\n"
		    "\n"
	            "       Examples :\n\n"
	            "          Encrypt 1.tgz to 1.tgz.cr\n"
		    "          %s -p \"this is a test\" -i 1.tgz -o 1.tgz.cr\n\n"
	            "          Decrypt 1.tgz.cr back to 1.tgz\n"
		    "          %s -p \"this is a test\" -o 1.tgz -i 1.tgz.cr\n\n"
	            "          Encrypt 'salut'\n"
		    "          %s -p \"this is a test\" -s salut\n\n"
	            "   Copyright (C) 2006, 2008 by GUILLAUME Nicolas\n"
	            "   ng@ngsoft-fr.com\n\n"

		    , pname, MAX_PASS, pname, pname, pname);
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

    FILE * pass_file = NULL;
    int key = 0;
    char * pass = NULL;
    char * string = NULL;
    FILE * in = NULL;
    FILE * out = NULL;
    char buff[MAX_PASS];

    while( (optch = getopt(argc, argv, "p:k:s:hvri:o:f:")) != -1 )
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
	case 'r':
	    fputs("warning: -r option is deprecated, please use -f - instead.\n", stderr);
	    pass_file = stdin;
	    break;
	case 'f':
	    if(!strcmp(optarg, "-"))
		pass_file = stdin;
	    else
	    {
		pass_file = fopen(optarg, "rb");
		if(!pass_file)
		{
		    fprintf(stderr, "%s: unable to open pass file `%s'\n", pname, optarg);
		    return EXIT_FAILURE;
		}
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
	case 'v':
	    version(pname);
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

    if(!pass_file && !pass)
    {
	fprintf(stderr, "%s: passphrase not defined\n", pname);
	return EXIT_FAILURE;
    }

    if(pass && pass_file)
    {
	fprintf(stderr, "%s: passphrase defined twice\n", pname);
	return EXIT_FAILURE;
    }

    if(pass_file == stdin && !in)
    {
	fprintf(stderr, "%s: cannot read both passphrase and input file from stdin\n", pname);
	return EXIT_FAILURE;
    }

    if(pass_file)
    {
	pass = fgets(buff, MAX_PASS, pass_file);
	if(!pass)
	{
	    fputs("unable to read passphrase from pass file/stdin!\n", stderr);
	    return EXIT_FAILURE;
	}
	fclose(pass_file);
    }

    if(!pass[0] || !pass[1])
    {
	fprintf(stderr, "%s: passphrase too small\n", pname);
	return EXIT_FAILURE;
    }

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

