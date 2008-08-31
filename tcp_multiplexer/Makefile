

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
CFLAGS=-W -Wall -Wno-unused -O3 $(ASSERTFLAGS) $(DDPFLAGS)
LDFLAGS=

all:
	$(CC) $(CFLAGS) -I../btun/ tcp_connection.c tcp_multiplexer.c ../btun/{common,md5,vector}.o -o tcpmux
	strip tcpmux

clean:
