
ifdef DISABLE_DEBUG_PRINT
    DDPFLAGS==-DDISABLE_DEBUG_PRINT=1
else
    DDPFLAGS=
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

CC=gcc
CFLAGS=-W -Wall -Wno-unused $(DEBUGFLAGS) $(ASSERTFLAGS) $(DDPFLAGS)
LDFLAGS=

SRC=common.c channel.c vector.c user.c cryptor.c ../xoror/xoror.c md5.c misc.c

SRC1=btund.c server.c $(SRC)
OBJS1=$(SRC1:.c=.o)
EXE1=btund

SRC2=btun.c client.c $(SRC)
OBJS2=$(SRC2:.c=.o)
EXE2=btun

all: $(EXE1) $(EXE2)

btun.o: assert.h bool.h client.h
btund.o: server.h user.h cryptor.h
channel.o: vector.h user.h bool.h channel.h server.h assert.h
client.o: bool.h common.h client.h assert.h
common.o: protocol.h common.h
cryptor.o: user.h cryptor.h ../xoror/xoror.h assert.h
server.o: user.h channel.h common.h server.h assert.h vector.h
user.o: vector.h bool.h protocol.h user.h assert.h
vector.o: vector.h assert.h

$(EXE1): $(OBJS1)
	$(CC) $(OBJS1) -o $@ $(LDFLAGS)
	$(STRIP) $@ > /dev/null

$(EXE2): $(OBJS2)
	$(CC) $(OBJS2) -o $@ $(LDFLAGS)
	$(STRIP) $@ > /dev/null

.PHONY: clean mrproper

clean:
	@rm -f *.o */*.o *~ core *.core core.* *.tmp

mrproper: clean
	@rm -f $(EXE1) $(EXE2)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

