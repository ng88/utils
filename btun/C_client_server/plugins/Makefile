
CC=gcc
CFLAGS=-W -Wall -Wno-unused -O3
LDFLAGS=-shared -fPIC

LIBEXT=so
LIBPREFIX=libbtp

all: $(LIBPREFIX)xoror.$(LIBEXT) $(LIBPREFIX)compress.$(LIBEXT) $(LIBPREFIX)tee.$(LIBEXT)


$(LIBPREFIX)xoror.$(LIBEXT): xoror.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(LIBPREFIX)xoror.$(LIBEXT) xoror.c ../../../xoror/xoror.c

$(LIBPREFIX)example.$(LIBEXT): example.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(LIBPREFIX)example.$(LIBEXT) example.c

$(LIBPREFIX)tee.$(LIBEXT): tee.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(LIBPREFIX)tee.$(LIBEXT) tee.c

$(LIBPREFIX)compress.$(LIBEXT): compress.c common.c
	$(CC) $(CFLAGS) $(LDFLAGS) -lz -o $(LIBPREFIX)compress.$(LIBEXT) compress.c common.c

$(LIBPREFIX)http.$(LIBEXT): http.c common.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(LIBPREFIX)http.$(LIBEXT) http.c common.c


clean:
	@rm -f *.o *.lo *.la *~

mrproper: clean
	@rm -f $(LIBPREFIX)*.$(LIBEXT)
