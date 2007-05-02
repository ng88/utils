
ifdef ENABLE_READLINE
    RD_LDF=-lreadline -ltermcap
    RD_CF=-DADV_READLINE
endif



CC=gcc
CFLAGS=-Wall $(RD_CF)
LDFLAGS=$(RD_LDF)

rsh: main.o conf.o rsh.o io.o

main.o: conf.h rsh.h io.h
conf.o: conf.h
rsh.o: conf.h rsh.h
io.o: io.h

.PHONY: clean

clean:
	rm -f *.o
