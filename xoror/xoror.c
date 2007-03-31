
#include <string.h>
#include <stdlib.h>

#include "xoror.h"


cryptor* cryptor_new(char * passphrase, int key)
{
    cryptor* ret = (cryptor*)malloc(sizeof(cryptor));

    ret->pos = 0;
    ret->rnd = 0;
    ret->passphrase = strdup(passphrase);
    ret->size = strlen(passphrase);

    ret->cseed = key;
    int i;
    for(i = 0; i < ret->size; ++i)
	ret->cseed += passphrase[i] ^ key;

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
    char ret;

    /* pseudo random number generator, RAND_MAX = 32767 */
    c->cseed = c->cseed * 1103515245 + 12345;
    c->rnd = ((unsigned)( c->cseed/65536) % 32768);

    if(c->pos >= c->size)
	c->pos = 0;

    ret = in ^ ( (c->rnd > 25000) ? c->passphrase[c->pos] : c->rnd );

    c->pos++;

    return ret;
}

void encrypt_file(FILE * in, FILE * out, cryptor * c)
{
    int ch;
    while( (ch = fgetc(in)) != EOF )
	fputc(cryptor_next(c, (char)ch), out);
}

void encrypt_string(char * in, char * out, cryptor * c)
{
    while( *in )
    {
	*out = cryptor_next(c, *in);
	in++; out++;
    }
}

