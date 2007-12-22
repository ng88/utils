#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>


#include "client.h"
#include "assert.h"
#include "misc.h"

int connect_to_server(char * server, port_t port,
		      char * login, char * pass,
		      char * channel, option_t options)
{

    int sockfd;
    struct sockaddr_in dest_addr;

    HANDLE_ERR(sockfd = socket(PF_INET, SOCK_STREAM, 0), "socket");

    struct hostent * h;

    if((h = gethostbyname(server)) == NULL)
    {
        herror("gethostbyname");
        return EXIT_FAILURE;
    }

    dbg_printf("trying to connect to %s (%s:%u)...\n", server, 
	       inet_ntoa(*((struct in_addr *)h->h_addr)), port);


    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    dest_addr.sin_addr = *((struct in_addr *)h->h_addr);
    memset(dest_addr.sin_zero, 0, sizeof(dest_addr.sin_zero));


    HANDLE_ERR(connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)), "connect");

    dbg_printf("connected.\n");

    /*     SEND LOGIN    */
    int n = min_u(strlen(login), (size_t)USER_MAX_LOGIN_SIZE);
    HANDLE_ERR(sendall(sockfd, login, &n), "sendall");


    /*    RECEIVE CHALLENGE   */
    char ch[CHALLENGE_SIZE];
    n = CHALLENGE_SIZE;
    HANDLE_ERR(recvall(sockfd, ch, &n), "recvall");

    dbg_printf("challenge received, answering...\n");

    /*    ANSWER CHALLENGE   */
    MD5_CTX_ppp m;
    challenge_answer(ch, pass, &m);
    n = 16;
    HANDLE_ERR(sendall(sockfd, m.digest, &n), "sendall");

    /*  RECEIVE AGREEMENT  */
    n = 1;
    HANDLE_ERR(recvall(sockfd, ch, &n), "recvall");
    if(ch[0])
    {
	fputs("login failed\n", stderr);
	dbg_printf("login failed, error %d\n", ch[0]);
	return EXIT_FAILURE;
    }

    dbg_printf("logged in, requesting channel `%s'...\n", channel);


    /*     SEND CHANNEL NAME    */
    n = min_u(strlen(channel), (size_t)USER_MAX_CHANNEL_SIZE);
    HANDLE_ERR(sendall(sockfd, channel, &n), "sendall");


    /*     SEND CHANNEL OPTIONS   */
    n = 1;
    HANDLE_ERR(sendall(sockfd, &options, &n), "sendall");


    /*  RECEIVE AGREEMENT  */
    n = 1;
    HANDLE_ERR(recvall(sockfd, ch, &n), "recvall");

    switch(ch[0])
    {
    case CA_GRANTED:
	dbg_printf("connected to channel.");
	break;
    case CA_DENIED:
	fprintf(stderr, "channel `%s': permission denied\n", channel);
	return EXIT_FAILURE;
    case CA_ALREADY_EXISTS:
	fprintf(stderr, "channel `%s' already exists, you can't be the master.\n", channel);
	return EXIT_FAILURE;
    default:
	fprintf(stderr, "can not connect to channel `%s', error %d\n", channel, ch[0]);
	return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
