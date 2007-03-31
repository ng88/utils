#ifndef XOROR_H
#define XOROR_H

/*
  Author: Nicolas GUILLAUME

  Xoror is a simple xor cryptor.

  Warning: This may NOT resist anybody.
           Do NOT use it for critical data.

*/


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

