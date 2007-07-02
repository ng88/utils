/***************************************************************************
 *   This file is part of the 'utils' projects                             *
 *                                                                         *
 *   'utils' projects                                                      *
 *                                                                         *
 *   Copyright (C) 2006, 2007 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://www.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/                                                                

#ifndef XOROR_H
#define XOROR_H

/*
  Author: Nicolas GUILLAUME

  Xoror is a simple xor cryptor.

  Warning: This may NOT resist anybody.
           Do NOT use it for critical data.

*/


#define LAST_ALGO_VERSION_MAJOR 0
#define LAST_ALGO_VERSION_MINOR 3
#define LAST_ALGO_VERSION (LAST_ALGO_VERSION_MAJOR * 1000 + LAST_ALGO_VERSION_MINOR)

#include <stdio.h>


typedef struct
{

    /* passphrase */
    char * passphrase;
    /* length of passphrase */
    size_t size;
    /* current position in passphrase */
    int pos;

    /* seed used by the  pseudo random number generator */
    int cseed;
    /* next  pseudo random number */
    unsigned int rnd;

} cryptor;

/* Create a new cryptor */
cryptor* cryptor_new(char * passphrase, int key);

/* Destroy c */
void cryptor_free(cryptor* c);

/* Use c to return the encrypted value of 'in' */
char cryptor_next(cryptor* c, char in);


/* Encrypt a file with c*/
void encrypt_file(FILE * in, FILE * out, cryptor * c);

/* Encrypt a string with c */
void encrypt_string(char * in, char * out, cryptor * c);





#endif

