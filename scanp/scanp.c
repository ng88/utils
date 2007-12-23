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
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_net.h>


/*

  gcc -Wall scanp.c  $(sdl-config --cflags) -lSDL -lSDL_net

*/

static unsigned short int STD_PORTS[] = {21, 22, 25, 37, 80, 110, 5800, 5801, 5900, 5901};

const unsigned int STD_PORTS_SIZE = sizeof(STD_PORTS) / sizeof(STD_PORTS[0]);


Uint32 times_up(Uint32 ts, void * o)
{
    printf("timesup\n");
    return 0;
}

int port_is_open_TCP(char * addr, unsigned int port)
{
    IPaddress ip;
    TCPsocket socket;
    int ret;
 
    SDLNet_ResolveHost(&ip, addr, port);
    socket = SDLNet_TCP_Open(&ip);

    ret = socket ? 1 : 0;

    SDLNet_TCP_Close(socket);
    return ret;
}



int main(int argc,char** argv)
{
    SDL_Init(SDL_INIT_TIMER);
    SDLNet_Init();
    atexit(SDLNet_Quit);
    atexit(SDL_Quit);

    /*SDL_TimerID timer;*/

    


    char * addr = argv[1];

    int i;

    for(i = 0; i < STD_PORTS_SIZE; ++i)
    {
	/*timer = SDL_AddTimer(500, times_up, NULL);*/
	printf("%s:%5d/TCP %d\n", addr, STD_PORTS[i], port_is_open_TCP(addr, STD_PORTS[i]));
	/*SDL_RemoveTimer(timer);*/
    }
    
    return EXIT_SUCCESS;

}

