

#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

int main(int argc, char ** argv)
{

    FILE * f = stdin;

    if(argc > 1)
    {
	f = fopen(argv[1], "r");
	if(!f)
	{
	    fprintf(stderr, "Impossible de lire le ficher `%s' !\n", argv[1]);
	    return EXIT_FAILURE;
	}

    }

    grid_t g;

    if(!ss_read_grid_from_file(f, &g))
    {
	fputs("Impossible de lire la grille !\n", stderr);

	fclose(f);
	return EXIT_FAILURE;
    }

    ss_print_grid(&g);

    if(ss_solve_grid(&g))
    {
	puts("Solution :");
	ss_print_grid(&g);
    }
    else
	puts("Grille impossible � r�soudre !");


    fclose(f);
    return EXIT_SUCCESS;

}
