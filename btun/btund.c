
#include <stdlib.h>
#include <stdio.h>
#include "server.h"



int main(int argc, char ** argv)
{

    int r = start_server(SERVER_DEFAULT_PORT);

    return r;
}
