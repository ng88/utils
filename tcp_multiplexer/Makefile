
ifdef IPV6
    IPF=-DIPV=6 -DIPV6=1
else
    IPF=-DIPV=4 -DIPV4=1
endif


ifdef DEBUGPRINT
    DDPFLAGS=
else
    DDPFLAGS=-DDISABLE_DEBUG_PRINT=1
endif

ifdef NOASSERT
    ASSERTFLAGS=-DDISABLE_ASSERT=1
else
    ASSERTFLAGS=
endif

CC=gcc
CFLAGS=-W -Wall -Wno-unused -O3 $(ASSERTFLAGS) $(DDPFLAGS) $(IPF)
LDFLAGS=

all:
	$(CC) $(CFLAGS) -I../btun/C_client_server/ tcp_connection.c tcp_multiplexer.c ../btun/C_client_server/{common,md5,vector}.o -o tcpmux
	strip tcpmux

clean:
