
#include <string.h>
#include "xoror.h"


cryptor* cryptor_new(char * passphrase, int key)
{
    cryptor* ret = (cryptor*)malloc(sizeof(cryptor));

    ret->pos = 0;
    ret->key = key;
    ret->passphrase = strdup(passphrase);

    return ret;
}

void cryptor_free(cryptor* c)
{
    if(!c) return;

    free(c->passphrase);
    free(c);
}

char cryptor_next(cryptor* c, char in)
{
}

int encrypt_file(FILE * in, FILE * out, cryptor * c)
{
    int c;
    while( (c = fgetc(in)) != EOF )
	fputc(cryptor_next(c, (char)c), out);
}

int encrypt_string(char * in, char * out, cryptor * c)
{
}

