
#include "sudoku.h"
#include <ctype.h>
#include <assert.h>



int ss_read_grid_from_file(FILE * f, grid_t * dest)
{

    int i = 0;
    int j;

    while(i < 9 && !feof(f))
    {

	j = 0;
	while(j < 9 && !feof(f))
	{
	    char c = fgetc(f);

	    ss_get_box(dest, i, j) = isdigit(c) ? c - '0' : B_UNKNOWN;

	    j++;
	}
	fgetc(f);
	i++;
    }

    return i == 9 && j == 9;

}

void print_line()
{
    int i;
    for(i = 0; i < 9+10; ++i)
	putchar('-');
    putchar('\n');
}

void ss_print_grid(grid_t * g)
{

    int i, j;

    print_line();

    for(i = 0; i < 9; ++i)
    {
	putchar('|');
	for(j = 0; j < 9; ++j)
	{
	    box_t c = ss_get_box(g, i, j);
	    putchar(c == B_UNKNOWN ? '.' : '0' + c);

	    if( (j+1) % 3 == 0)
		putchar('|');
	    else
		putchar(' ');
	}

	putchar('\n');

	if( (i+1) % 3 == 0)
	    print_line();
    }

}

void ss_goto_next_box(grid_t * g)
{
    g->pos.x++;

    if(g->pos.x == 9)
    {
	g->pos.x = 0;
	g->pos.y++;
	assert(g->pos.y < 9);
    }
}

void ss_backto_previous_box(grid_t * g)
{

    g->pos.x--;

    if(g->pos.x == -1)
    {
	g->pos.x = 9 - 1;
	g->pos.y--;
	assert(g->pos.y > -1);
    }
}

int ss_is_a_valid_possibility(grid_t * g, box_t v)
{
    int i, j;

    /* existe t-il deja sur la ligne ? */
    for(j = 0; j < 9; ++j)
	if(ss_get_box(g, g->pos.x, j) == v)
	    return 0;

    /* existe t-il deja sur la colonne ? */
    for(i = 0; i < 9; ++i)
	if(ss_get_box(g, i, g->pos.y) == v)
	    return 0;

    /* existe t-il deja dans son sous carré ? */
    int square_x = g->pos.x / 3;
    int square_y = g->pos.y / 3;

    for(i = 0; i < 3; ++i)
	for(j = 0; j < 3; ++j)
	    if(ss_get_box(g, square_x + i, square_y + j) == v)
		return 0; 

    return 1;
    
}

void ss_solve_grid(grid_t * g)
{
    g->pos.x = 0;
    g->pos.y = 0;
    g->success = 0;

    ss_solve_grid_next(g);
}

void ss_solve_grid_next(grid_t * g)
{


    box_t c = ss_get_box_c(g, g->pos);

    if(c == B_UNKNOWN) /* case inconnue */
    {
	/* pour chaque chiffre possible */
    }
    else /* case connue */
    {
	ss_goto_next_box(g);
	ss_solve_grid_next(g);

	if(g->success) /* c'est gagné ! */
	    return;

	ss_backto_previous_box(g);

    }


}



