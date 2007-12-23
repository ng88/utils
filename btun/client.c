#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#include <fcntl.h>
#include <termios.h>


#include "client.h"
#include "assert.h"
#include "misc.h"

#define RECV_BUFF_SIZE 256


bool run;
pid_t ch_pid;

int connect_to_server(char * server, port_t port,
		      char * login, char * pass,
		      char * channel, option_t options,
		      char * cmd)
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
    int n = min_u(strlen(login) + 1, (size_t)USER_MAX_LOGIN_SIZE);
    HANDLE_ERR(sendall(sockfd, login, &n), "sendall");


    /*    RECEIVE CHALLENGE   */
    char ch[  MMAX(CHALLENGE_SIZE, USER_MAX_CHANNEL_SIZE + 1)  ];
    n = CHALLENGE_SIZE;
    HANDLE_ERR(recvall(sockfd, ch, &n), "recvall");

    dbg_printf("challenge received, answering...\n");

    /*    ANSWER CHALLENGE   */
    MD5_CTX_ppp m;
    challenge_answer(ch, pass, &m);
   memset(pass, '*', strlen(pass));
    n = MD5_SIZE;
    HANDLE_ERR(sendall(sockfd, m.digest, &n), "sendall");

    /*  RECEIVE AGREEMENT  */
    n = 1;
    HANDLE_ERR(recvall(sockfd, ch, &n), "recvall");
    if(ch[0] != UA_GRANTED)
    {
	fputs("login failed\n", stderr);
	dbg_printf("login failed, error %d\n", ch[0]);
	return EXIT_FAILURE;
    }

    dbg_printf("logged in, requesting channel `%s'...\n", channel);


    /*     SEND CHANNEL NAME    */
    n = min_u(strlen(channel) + 1, (size_t)USER_MAX_CHANNEL_SIZE - 1);
    strncpy(ch, channel, n);
    ch[n++] = (char)options;
    HANDLE_ERR(sendall(sockfd, ch, &n), "sendall");


    /*  RECEIVE AGREEMENT  */
    n = 1;
    HANDLE_ERR(recvall(sockfd, ch, &n), "recvall");

    switch(ch[0])
    {
    case CA_GRANTED:
	dbg_printf("connected to channel.\n");
	break;
    case CA_DENIED:
	fprintf(stderr, "channel `%s': permission denied\n", channel);
	return EXIT_FAILURE;
    case CA_CANT_BE_MASTER:
	fprintf(stderr, "channel `%s' already exists, you can't be the master.\n", channel);
	return EXIT_FAILURE;
    case CA_TOO_MUCH_CHANNEL:
	fprintf(stderr, "can't add channel `%s', too much channels !\n", channel);
	return EXIT_FAILURE;
    default:
	fprintf(stderr, "can not connect to channel `%s', error %d\n", channel, ch[0]);
	return EXIT_FAILURE;
    }


    run = true;
    ch_pid = 0;

    if(cmd)
	run_with_prog_on_pty(sockfd, cmd);
    else
	run_normal(sockfd, 0, 1);

    dbg_printf("client halted.\n");

    return EXIT_SUCCESS;
}

void stop_client()
{
    if(ch_pid)
    {
	kill(ch_pid, SIGTERM);
	usleep(100*1000);
	kill(ch_pid, SIGKILL);
	usleep(100*1000);
    }
    run = false;
}

void run_normal(int sockfd, int in, int out)
{
    fd_set fds;

    char buf[RECV_BUFF_SIZE];
    int n;

    int fdmax = max_s(sockfd, in);

    while(run)
    {
	FD_ZERO(&fds);
	FD_SET(in, &fds); /* stdin */
	FD_SET(sockfd, &fds); /* socket */

        if(select(fdmax + 1, &fds, NULL, NULL, NULL) == -1)
	    run = false;
	else
	{
	    if(FD_ISSET(in, &fds)) /* stdin */
	    {
		n = read(in, buf, RECV_BUFF_SIZE);
		if(n <= 0 || sendall(sockfd, buf, &n) == -1)
		    run = false;
	    }

	    if(run && FD_ISSET(sockfd, &fds))
	    {
		n = recv(sockfd, buf, RECV_BUFF_SIZE, MSG_NOSIGNAL);
		if(n <= 0 || writeall(out, buf, n) == -1)
		    run = false;
	    }


	}
    }
}



void run_with_prog(int sockfd, char * p)
{

    ch_pid = fork();

    if(ch_pid == 0) /* fils */
    {
	close(0);
	close(1);
	close(2);
	dup(sockfd);
	dup(sockfd);
	dup(sockfd);
	close(sockfd);
	execlp(p, p, NULL);
	perror("exec");
	_exit(1);
    }
    else if(ch_pid > 0) /* pere */
    {
	wait(NULL);
    }
    else
	perror("fork");

}

void run_with_prog_on_pty(int sockfd, char * p)
{
    int fdm, fds, rc;

    if( (fdm = posix_openpt(O_RDWR)) < 0 )
    {
	perror("openpt");
	return;
    }

    if( (rc = grantpt(fdm)) != 0)
    {
	perror("grantpt");
	return;
    }

    if( (rc = unlockpt(fdm)) != 0)
    {
	perror("unlockpt");
	return;
    }

    fds = open(ptsname(fdm), O_RDWR);

    ch_pid = fork();

    if(ch_pid == 0) /* fils */
    {

	close(fdm);

	struct termios settings;

	rc = tcgetattr(fds, &settings);
	cfmakeraw(&settings);
	tcsetattr(fds, TCSANOW, &settings);

	rc = tcgetattr(fds, &settings);
	cfmakeraw(&settings);
	tcsetattr(fds, TCSANOW, &settings);

	close(0);
	close(1);
	close(2);
	dup(fds);
	dup(fds);
	dup(fds);

	close(sockfd);

	setsid();

	execlp(p, p, NULL);
	perror("exec");
	_exit(1);
    }
    else if(ch_pid > 0) /* pere */
    {

	close(fds);

	run_normal(sockfd, fdm, fdm);

	wait(NULL);
    }
    else
	perror("fork");

}

int writeall(int fd, void * src, size_t s)
{
    while(s)
    {
	int lu = write(fd, src, s);

	if(lu <= 0)
	    return -1;

	src += lu;
	s -= lu;
    }

    return 0;
}
