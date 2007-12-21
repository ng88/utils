
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
CFLAGS=-W -Wall -Wno-unused $(DEBUGFLAGS) $(ASSERTFLAGS)
LDFLAGS=

SRC=channel.c server.c vector.c user.c cryptor.c ../xoror/xoror.c

SRC1=btund.c $(SRC)
OBJS1=$(SRC1:.c=.o)
EXE1=btund

SRC2=btun.c $(SRC)
OBJS2=$(SRC2:.c=.o)
EXE2=btun

all: $(EXE1) $(EXE2)

btund.o: server.h user.h
channel.o: vector.h user.h channel.h server.h assert.h
cryptor.o: user.h cryptor.h ../xoror/xoror.h assert.h
server.o: server.h assert.h channel.h
user.o: vector.h bool.h user.h assert.h
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

