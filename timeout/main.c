#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


void timeout(int s)
{
    kill(0, SIGTERM);
    usleep(1000*1000);
    kill(0, SIGKILL);
    exit(2);
}

int main(int argc, char ** argv)
{

    if(argc != 3)
    {
	fputs("usage: timeout delay command\n", stderr);
	return EXIT_FAILURE;
    }

    int delay = atoi(argv[1]);

    struct sigaction nv, old;
    memset(&nv, 0, sizeof(nv));
    nv.sa_handler = &timeout;
    sigaction(SIGALRM, &nv, &old);

    alarm(delay);

    return system(argv[2]);
}
