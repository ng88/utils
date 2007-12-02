


CC=gcc
CFLAGS=-Wall
LDFLAGS=

sudoku-solver: main.o sudoku.o
	gcc main.o sudoku.o -o sudoku-solver

main.o: sudoku.h
sudoku.o: sudoku.h

.PHONY: clean

clean:
	rm -f *.o
