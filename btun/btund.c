
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "server.h"


void stop_server_handler(int s)
{
    stop_server();
}

int main(int argc, char ** argv)
{

    struct sigaction nv, old;
    memset(&nv, 0, sizeof(nv));
    nv.sa_handler = &stop_server_handler;

    sigaction(SIGTERM, &nv, &old);
    sigaction(SIGINT, &nv, &old);

    int r = start_server(SERVER_DEFAULT_PORT);

    return r;
}
