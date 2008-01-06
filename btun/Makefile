
ifdef DISABLE_DEBUG_PRINT
    DDPFLAGS=-DDISABLE_DEBUG_PRINT=1
else
    DDPFLAGS=
endif

ifdef DONT_USE_LL
    DULL=
else
    DULL=-DUSE_LL=1
endif

ifdef DEBUG
    DEBUGFLAGS=-g -ggdb -dH -D_DEBUG_=1 -DDEBUG=1
    STRIP=@echo
else
    DEBUGFLAGS=
    STRIP=strip
endif

ifdef NOASSERT
    ASSERTFLAGS=-DDISABLE_ASSERT=1
else
    ASSERTFLAGS=
endif

ifdef REV
    REVFLAGS=-D__SVN_HEAD__=\"$(REV)\"
else
    REVFLAGS=-D__SVN_HEAD__=\"`cat last_svn_rev~`\"
endif

CC=gcc
CFLAGS=-W -Wall -Wno-unused $(REVFLAGS) $(DEBUGFLAGS) $(ASSERTFLAGS) $(DDPFLAGS) $(DULL)
LDFLAGS=

SRC=common.c vector.c md5.c misc.c

SRC1=btund.c server.c stats.c channel.c user.c $(SRC)
OBJS1=$(SRC1:.c=.o)
EXE1=btund

SRC2=btun.c client.c $(SRC)
OBJS2=$(SRC2:.c=.o)
EXE2=btun

all: $(EXE1) $(EXE2)

btun.o: assert.h bool.h client.h version.h protocol.h misc.h
btund.o: server.h user.h version.h protocol.h misc.h bool.h assert.h
channel.o: vector.h user.h bool.h channel.h server.h assert.h
client.o: bool.h common.h client.h assert.h misc.h
common.o: protocol.h md5.h common.h bool.h assert.h
md5.o: md5.h assert.h
misc.o: bool.h misc.h
server.o: user.h channel.h common.h server.h assert.h vector.h stats.h
stats.o: bool.h stats.h assert.h
user.o: vector.h bool.h protocol.h user.h assert.h
vector.o: vector.h assert.h

$(EXE1): svnrev $(OBJS1)
	$(CC) $(OBJS1) -o $@ $(LDFLAGS)
	$(STRIP) $@ > /dev/null

$(EXE2): svnrev $(OBJS2)
	$(CC) $(OBJS2) -o $@ $(LDFLAGS)
	$(STRIP) $@ > /dev/null

.PHONY: clean mrproper svnrev

svnrev:
	@sed -n 's/.*revision="\([^"]*\)".*/\1/gp' .svn/entries | sort -nr | head -1 > last_svn_rev~

clean:
	@rm -f *.o */*.o *~ core *.core core.* *.tmp

mrproper: clean
	@rm -f $(EXE1) $(EXE2)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

