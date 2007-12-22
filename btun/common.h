#ifndef COMMON_H
#define COMMON_H

#include "protocol.h"
#include "md5.h"



/** Send all the 'size' bytes of 'buff' to 'fd' */
int sendall(int fd, char * buff, int * size);

/** Receive 'size' bytes from 'fd' to 'buff' */
int recvall(int fd, char * buff, int * size);

/** Compute the challenge answer */
void challenge_answer(char * challenge, char * userpwd, MD5_CTX_ppp * m);


#define HANDLE_ERR(v, str) \
    { if( (v) == -1 ) { perror(str); return EXIT_FAILURE; } }

#endif
