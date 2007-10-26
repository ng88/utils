


CC=gcc
CFLAGS=-Wall
LDFLAGS=

sudokusolver: main.o sudoku.o
	gcc main.o sudoku.o -o sudokusolver

main.o: sudoku.h
sudoku.o: sudoku.h

.PHONY: clean

clean:
	rm -f *.o
