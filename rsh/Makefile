
CC=gcc
CFLAGS=-Wall

rsh: main.o conf.o rsh.o

main.o: conf.h rsh.h
conf.o: conf.h
rsh.o: conf.h rsh.h

.PHONY: clean

clean:
	rm -f *.o
