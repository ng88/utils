
CC=gcc
CFLAGS=-W -Wall -Wno-unused -O3
LDFLAGS=
LCTOOL=libtool --mode=compile $(CC)
LTTOOL=libtool --mode=link $(CC)
RPATH=/usr/local/lib

LIBEXT=so
#LIBEXT=dll

all: xoror.$(LIBEXT)


xoror.$(LIBEXT): xoror.c
	$(LCTOOL) $(CFLAGS) $(LDFLAGS) -c xoror.c -c ../../xoror/xoror.c
	$(LTTOOL) $(CFLAGS) $(LDFLAGS) -module -o libxoror.la xoror.lo -rpath $(RPATH)
	cp .libs/libxoror.$(LIBEXT) xoror.$(LIBEXT)

clean:
	@rm -f *.o *.lo *.la *~
	@rm -fr .libs

mrproper: clean
	@rm -f *.$(LIBEXT) *.dll
