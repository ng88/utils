
CC=gcc
CFLAGS=-W -Wall -Wno-unused -O3
LDFLAGS=

LIBEXT=so

all: xoror.$(LIBEXT)


xoror.$(LIBEXT): xoror.c
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -fPIC -o xoror.so xoror.c ../../xoror/xoror.c

example.$(LIBEXT): example.c
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -fPIC -o example.so example.c



clean:
	@rm -f *.o *.lo *.la *~

mrproper: clean
	@rm -f *.$(LIBEXT)
