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

#ifndef COMMON_H
#define COMMON_H

#include "protocol.h"
#include "md5.h"



/** Send all the 'size' bytes of 'buff' to 'fd' */
int sendall(int fd, char * buff, int * size);

/** Receive 'size' bytes from 'fd' to 'buff' */
int recvall(int fd, char * buff, int * size);

char * create_challenge();

/** Compute the challenge answer */
void challenge_answer(char * challenge, char * userpwd, MD5_CTX_ppp * m);


#define HANDLE_ERR(v, str) \
    { if( (v) == -1 ) { perror(str); return EXIT_FAILURE; } }

#endif
