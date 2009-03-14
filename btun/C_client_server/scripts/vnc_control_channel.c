/*
 *   Same thing as vnc_control_channel.sh but in C :)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum
{
    STRBUFFSIZE = 128,
    SYSBUFFSIZE = 1024,
};

typedef enum
{
    RW_FAILED = 0,
    RW_SPACE = 1,
    RW_BREAK = 2,
} rw_t;

/** Read a word, return RW_FAILED on failure, RW_SPACE if another word follows
 * on the same line, RW_BREAK otherwise
 */
rw_t readword(char * dst);



int main(int argc, char ** argv)
{

    char cmd[STRBUFFSIZE];
    char arg0[STRBUFFSIZE];

    char sys[SYSBUFFSIZE];

    if(argc != 7)
    {
	fprintf(stderr, "usage: %s username passfile btunserver vnc_host vnc_port machine\n", argv[0]);
	return EXIT_FAILURE;
    }

    char * username = argv[1];
    char * passfile = argv[2];
    char * server = argv[3];
    char * vhost = argv[4];
    char * vport = argv[5];
    char * machine = argv[6];

    int lastid = 200;

    srand(time(0));

    while(!feof(stdin))
    {
	arg0[0] = '\0';
    
	if(readword(cmd) == RW_SPACE)
	    readword(arg0);

	if(!strcmp(cmd, "GetNewChannelName"))
	{
	    lastid++;
	    printf("%s_%s_%s%d_%d%d\n", username, machine,
		   arg0[0] ? arg0 : "MSC",
		   lastid, rand(), rand());

	}
	else if(!strcmp(cmd, "ConnectToVNC"))
	{
	    if(!arg0[0])
		puts("Error: missing channel name argument");
	    else
	    {
		snprintf(sys, SYSBUFFSIZE, "btun -d -f %s %s@%s %s nc %s %s", passfile, username, server, arg0, vhost, vport);
		system(sys);
		puts("Ok");
	    }
	}

    }

    return EXIT_SUCCESS;
}

rw_t readword(char * dst)
{
    int i = 0;
    int c;
    rw_t ret = RW_FAILED;

    while(ret == RW_FAILED && i < STRBUFFSIZE - 4 && (c = fgetc(stdin)) != EOF)
    {
	char cc = (char)c;
	if(cc == ' ')
	    ret = RW_SPACE;
	else if(cc == '\n')
	    ret = RW_BREAK;
	else
	{
	    dst[i] = cc;
	    i++;
	}
    }
    dst[i] = '\0';
    dst[STRBUFFSIZE - 1] = '\0';
    return ret;
}


