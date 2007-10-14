

#include <stdio.h>
#include <stdlib.h>

#include "sudoku.h"

int main()
{

    grid_t g;

    if(!ss_read_grid_from_file(stdin, &g))
    {
	fputs("impossible de lire la grille !\n", stderr);
	return EXIT_FAILURE;
    }

    ss_print_grid(&g);



    return EXIT_SUCCESS;

}
