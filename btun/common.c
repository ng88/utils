#include "common.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

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


void challenge_answer(char * ch, char * pass, MD5_CTX_ppp * m)
{
    MD5Init_ppp(m);

    MD5Update_ppp(m, ch, CHALLENGE_SIZE);
    MD5Update_ppp(m, pass, strlen(pass));
    MD5Update_ppp(m, ch, CHALLENGE_SIZE);

    MD5Final_ppp(m);

}

