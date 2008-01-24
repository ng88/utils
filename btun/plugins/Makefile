
CC=gcc
CFLAGS=-W -Wall -Wno-unused -O3
LDFLAGS=-shared -fPIC

LIBEXT=so

all: xoror.$(LIBEXT) compress.$(LIBEXT)


xoror.$(LIBEXT): xoror.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o xoror.so xoror.c ../../xoror/xoror.c

example.$(LIBEXT): example.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o example.so example.c

compress.$(LIBEXT): compress.c
	$(CC) $(CFLAGS) $(LDFLAGS) -lz -o compress.so compress.c



clean:
	@rm -f *.o *.lo *.la *~

mrproper: clean
	@rm -f *.$(LIBEXT)
