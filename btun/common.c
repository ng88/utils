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

#include "common.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "bool.h"
#include "assert.h"

int sendall(int fd, char * buff, int * size)
{
    int total = 0;
    int bytesleft = *size;
    int n;

    while(total < *size)
    {
        n = send(fd, buff + total, bytesleft, MSG_NOSIGNAL);

        if (n == -1)
	    break;

        total += n;
        bytesleft -= n;
    }

    *size = total;

    return n == -1 ? -1 : 0;
} 


int recvall(int fd, char * buff, int * size)
{
    int total = 0;
    int bytesleft = *size;
    int n;

    while(total < *size)
    {
        n = recv(fd, buff + total, bytesleft, MSG_NOSIGNAL);

        if (n == -1)
	    break;

        total += n;
        bytesleft -= n;
    }

    *size = total;

    return n == -1 ? -1 : 0;
} 

char * create_challenge()
{

    static bool init = false;

    if(!init)
    {
	srand(time(0));
	init = true;
    }

    char buff[CHALLENGE_SIZE];

    int i = 0;
    while(i < CHALLENGE_SIZE)
	buff[i++] = (char)(1 + rand() % 253);

    char * r = strdup(buff);

    c_assert(r);

    return r;
}

void challenge_answer(char * ch, char * pass, MD5_CTX_ppp * m)
{
    MD5Init_ppp(m);

    MD5Update_ppp(m, ch, CHALLENGE_SIZE);
    MD5Update_ppp(m, pass, strlen(pass));
    MD5Update_ppp(m, ch, CHALLENGE_SIZE);

    MD5Final_ppp(m);
}

