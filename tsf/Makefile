CC=gcc
CFLAGS=-W -Wall -O2
LDFLAGS=-lz
BIN=tsf

OBJS=tsf.o main.o

all: $(BIN)

tsf.o: tsf.h
main.o: tsf.h

$(BIN): $(OBJS)

.PHONY: clean mrproper


clean:
	@rm -f *.o *~ core *.core core.* *.tmp

mrproper: clean
	@rm -f $(BIN)
