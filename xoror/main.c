
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "xoror.h"


/*
 
     Compile with:  gcc -Wall xoror.c main.c -o xoror

*/


/* -h -p blabla -k 456 -s salut|-i file1 -o file2*/
void usage(const char * pname)
{
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
	    if(!in)
	    {
		fprintf(stderr, "%s: unable to open file `%s'\n", pname, optarg);
		return EXIT_FAILURE;
	    }
	    break;
	case 'i':
	    pass = optarg;
	    break;
	case 'k':
	    key = atoi(optarg);
	    break;
	case 's':
	    string = optarg;
	    break;
	case 'h':
	    usage(pname);
	    break;
	default:
	    break;
	}
    }

    if( (in || out) && string)
	usage(pname);

    if( !pass || strlen(pass) <= 2)
	usage(pname);

    if(!key)
	key = pass[0] * pass[1];

    printf("%d\n", argc - optind);

    return EXIT_SUCCESS;

    /*cryptor * c = cryptor_new(argv[1], atoi(argv[2]));

    FILE * in = fopen(argv[3], "rb");
    FILE * out = fopen(argv[4], "wb");


    encrypt_file(in, out, c);

    fclose(in);
    fclose(out);

    cryptor_free(c);

    return EXIT_SUCCESS;*/
}

