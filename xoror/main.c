
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
		    "       -o file           output file, cannot be used with -s\n"
		    "                         if not specified, stdout is used\n"
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
		fprintf(stderr, "%s: unable to open file `%s'\n", pname, optarg);
		return EXIT_FAILURE;
	    }
	    break;
	case 'o':
	    out = fopen(optarg, "wb");
	    if(!out)
	    {
		fprintf(stderr, "%s: unable to open file `%s'\n", pname, optarg);
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
		fprintf(stderr, "%s: invalid option `%c'\n", pname, optch);
	    usage(pname, EXIT_FAILURE);
	    break;
	}
    }

    if(argc - optind)
	usage(pname, EXIT_FAILURE);

    if( (in || out) && string)
	usage(pname, EXIT_FAILURE);

    if( !pass || strlen(pass) <= 2)
	usage(pname, EXIT_FAILURE);

    if(!key)
	key = pass[0] * pass[1];

    cryptor * c = cryptor_new(argv[1], atoi(argv[2]));

    if(string) /* working with string */
    {
	char * out = (char*)malloc(strlen(string) + 1);

	encrypt_string(string, out, c);
	puts(out);

	free(out);
    }
    else /* working with files */
    {
	if(!in) in = stdin;
	if(!out) out = stdout;

 
	encrypt_file(in, out, c);

	fclose(in);
	fclose(out);

    }

    cryptor_free(c);

    return EXIT_SUCCESS;

}

