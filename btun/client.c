/***************************************************************************
 *   This file is part of the 'utils' projects                             *
 *                                                                         *
 *   'utils' projects                                                      *
 *                                                                         *
 *   Copyright (C) 2006, 2008 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://svn.ngsoft-fr.com/trac/utils/                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/                                                                


#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#include <fcntl.h>
#include <termios.h>

#define _XOPEN_SOURCE_
#define __USE_XOPEN
#define __USE_XOPEN2K
#include <stdlib.h>

#include "client.h"
#include "assert.h"
#include "misc.h"

#define RECV_BUFF_SIZE 256


static bool run;
static pid_t ch_pid;

int connect_to_server(char * server, port_t port,
		      char * login, char * pass,
		      char * channel, option_t options,
		      mode_t mode, char ** cmd_args)
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
    if(sendall(sockfd, login, &n) == -1)
    {
	perror("login");
	close(sockfd);
	return EXIT_FAILURE;
    }


    /*    RECEIVE CHALLENGE   */
    char ch[  MMAX(CHALLENGE_SIZE, USER_MAX_CHANNEL_SIZE + 1)  ];
    n = CHALLENGE_SIZE;
    if(recvall(sockfd, ch, &n) == -1)
    {
	perror("rchallenge");
	close(sockfd);
	return EXIT_FAILURE;
    }

    dbg_printf("challenge received, answering...\n");

    /*    ANSWER CHALLENGE   */
    MD5_CTX_ppp m;
    challenge_answer(ch, pass, &m);
    memset(pass, '*', strlen(pass));
    n = MD5_SIZE;
    if(sendall(sockfd, m.digest, &n) == -1)
    {
	perror("achallenge");
	close(sockfd);
	return EXIT_FAILURE;
    }

    /*  RECEIVE AGREEMENT  */
    n = 1;
    if(recvall(sockfd, ch, &n) == -1)
    {
	perror("challenge agreement");
	close(sockfd);
	return EXIT_FAILURE;
    }

    if(ch[0] != UA_GRANTED)
    {
	fputs("login failed\n", stderr);
	dbg_printf("login failed, error %d\n", ch[0]);
        close(sockfd);
	return EXIT_FAILURE;
    }

    dbg_printf("logged in, requesting channel `%s'...\n", channel);


    /*     SEND CHANNEL NAME    */
    n = min_u(strlen(channel) + 1, (size_t)USER_MAX_CHANNEL_SIZE - 1);
    strncpy(ch, channel, n);
    ch[n++] = (char)options;
    if(sendall(sockfd, ch, &n) == -1)
    {
	perror("channel");
	close(sockfd);
	return EXIT_FAILURE;
    }


    /*  RECEIVE AGREEMENT  */
    n = 1;
    if(recvall(sockfd, ch, &n) == -1)
    {
	perror("channel agreement");
	close(sockfd);
	return EXIT_FAILURE;
    }

    switch(ch[0])
    {
    case CA_GRANTED:
	dbg_printf("connected to channel.\n");
	break;
    case CA_DENIED:
	fprintf(stderr, "can't join channel `%s': permission denied\n", channel);
        close(sockfd);
	return EXIT_FAILURE;
    case CA_CANT_BE_MASTER:
	fprintf(stderr, "channel `%s' already exists, can't be the master.\n", channel);
        close(sockfd);
	return EXIT_FAILURE;
    case CA_CANT_CHPERM:
	fprintf(stderr, "channel `%s' already exists, can't change permissions/options.\n", channel);
        close(sockfd);
	return EXIT_FAILURE;
    case CA_TOO_MUCH_CHANNEL:
	fprintf(stderr, "can't create channel `%s', too much channels!\n", channel);
        close(sockfd);
	return EXIT_FAILURE;
    case CA_TOO_MUCH_USER:
	fprintf(stderr, "can't join channel `%s', too much users!\n", channel);
        close(sockfd);
	return EXIT_FAILURE;
    default:
	fprintf(stderr, "can't join channel `%s', error %d\n", channel, ch[0]);
        close(sockfd);
	return EXIT_FAILURE;
    }


    run = true;
    ch_pid = 0;

    switch(mode)
    {
    case M_NORMAL:
	run_normal(sockfd, 0, 1);
	break;
    case M_EXEC_CMD:
	run_cmd(sockfd, cmd_args);
	break;
    case M_EXEC_CMD_PTY:
	run_cmd_pty(sockfd, cmd_args);
	break;
    }

    close(sockfd);
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



void run_cmd(int sockfd, char ** args)
{

    c_assert(args && args[0]);

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
	execvp(args[0], args);
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

void run_cmd_pty(int sockfd, char ** args)
{

    c_assert(args && args[0]);

    int fdm, fds;

    if( (fdm = posix_openpt(O_RDWR)) < 0 )
    {
	perror("openpt");
	return;
    }

    if( grantpt(fdm) != 0)
    {
	perror("grantpt");
	return;
    }

    if( unlockpt(fdm) != 0)
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

	if( tcgetattr(fds, &settings) == -1 )
	{
	    perror("tcgetattr");
	    return;
	}

	cfmakeraw(&settings);

	if( tcsetattr(fds, TCSANOW, &settings) == -1 )
	{
	    perror("tcsetattr");
	    return;
	}


	close(0);
	close(1);
	close(2);
	dup(fds);
	dup(fds);
	dup(fds);

	close(sockfd);

	if(setsid() == -1)
	{
	    perror("setsid");
	    return;
	}

	execvp(args[0], args);
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


char * read_passphrase(char * buff, size_t size)
{
    c_assert(buff);

    fputs("Enter passphrase: ", stdout);
    fflush(stdout);

    char * ret = buff;

    struct termios settings;

    if( tcgetattr(1, &settings) == -1 )
    {
	perror("tcgetattr");
	return NULL;
    }

    settings.c_lflag &= ~ECHO;

    if( tcsetattr(1, TCSANOW, &settings) == -1 )
    {
	perror("tcsetattr");
	return NULL;
    }

    if(fgets(buff, size, stdin))
    {
	buff[size - 1] = '\0';
	char * p = strchr(buff, '\n');
	if(p) *p = '\0';
    }
    else
	ret = NULL;

    settings.c_lflag |= ECHO;

    if( tcsetattr(1, TCSANOW, &settings) == -1 )
    {
	perror("tcsetattr");
	return NULL;
    }
    
    putchar('\n');

    return ret;
}



void flush_std()
{
    fflush(stdout);
    fflush(stderr);

/*  
    ungetc('*', stdin); 
    int ch;
    while( (ch = getc(stdin)) != EOF && ch != '\n');
*/

}
