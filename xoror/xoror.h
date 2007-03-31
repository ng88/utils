#ifndef XOROR_H
#define XOROR_H

/*
  Author: Nicolas GUILLAUME

  Xoror is a simple xor cryptor.

  Warning: This may NOT resist anybody.
           Do NOT use it for critical data (prefer something like AES).

*/


#include <stdio.h>


typedef struct
{
    int pos;

    char * passphrase;
    int key;

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

