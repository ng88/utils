
CC=gcc
CFLAGS=-Wall $(sdl-config --cflags) 
LDFLAGS=$(sdl-config --libs) -lSDL_net -lSDL

scanp: scanp.o


.PHONY: clean

clean:
	rm -f *.o
