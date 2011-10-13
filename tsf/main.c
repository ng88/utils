/**
 * Tar Stream Format
 * Tar-like archiving that can be easily streamed over HTTP
 *
 * Author: Nicolas GUILLAUME
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tsf.h"


void usage(const char * p)
{
    fprintf(stderr,
	    "usages: %s c destination.tsf files\n"
	    "        %s x destination.tsf [folder]\n",
	    p, p);
    exit(EXIT_FAILURE);
}

void extract_archive(const char * archive, const char * dir)
{
    printf("not yet implemented!\n");
}

void create_archive(const char * archive, char * files[], int file_count)
{
    int i;
    printf("create %s %d\n", archive, file_count);
    for(i = 0; i < file_count; i++)
	printf(" => %s\n", files[i]);
}

int main(int argc, char * argv[])
{
    if(argc < 3)
	usage(argv[0]);

    if(strlen(argv[1]) != 1)
	usage(argv[0]);

    const char * archive = argv[2];

    switch(argv[1][0])
    {
    case 'x':
    case 'X':	
	extract_archive(archive, argc < 4 ? "." : argv[3]);
	break;

    case 'c':
    case 'C':
	if(argc < 4)
	    usage(argv[0]);

	create_archive(archive, &argv[3], argc - 3);
	break;
    }

    return EXIT_SUCCESS;
}
