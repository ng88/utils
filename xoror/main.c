
#include <stdlib.h>
#include <string.h>
#include "xoror.h"


/*
 
     Compile with:  gcc -Wall xoror.c main.c -o xoror

*/ 

int main(int argc, char ** argv)
{

    cryptor * c = cryptor_new(argv[1], atoi(argv[2]));

    FILE * in = fopen(argv[3], "rb");
    FILE * out = fopen(argv[4], "wb");


    encrypt_file(in, out, c);

    fclose(in);
    fclose(out);

    cryptor_free(c);

    return EXIT_SUCCESS;
}

