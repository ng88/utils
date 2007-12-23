/***************************************************************************
 *   This file is part of the 'utils' projects                             *
 *                                                                         *
 *   'utils' projects                                                      *
 *                                                                         *
 *   Copyright (C) 2006, 2008 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://svn.ngsoft-fr.com/trac/utils/                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/

#include "sudoku.h"
#include <ctype.h>
#include <assert.h>



int ss_read_grid_from_file(FILE * f, grid_t * dest)
{

    int i;
    int j = 0;

    while(j < B_SIZE && !feof(f))
    {

	i = 0;
	while(i < B_SIZE && !feof(f))
	{
	    char c = fgetc(f);

	    ss_get_box(dest, i, j) = isdigit(c) ? c - '0' : B_UNKNOWN;
	    ss_get_known(dest, i, j) = (ss_get_box(dest, i, j) != B_UNKNOWN);

	    i++;
	}
	fgetc(f);
	j++;
    }

    return i == B_SIZE && j == B_SIZE;

}

void print_line()
{  
    int i;
    for(i = 0; i < 1 + 2 * B_SIZE; ++i)
	putchar('-');

    putchar('\n');
}

void ss_print_grid_text(grid_t * g)
{

    int i, j;

    print_line();

    for(j = 0; j < B_SIZE; ++j)
    {
	putchar('|');
	for(i = 0; i < B_SIZE; ++i)
	{
	    box_t c = ss_get_box(g, i, j);
	    putchar(ss_box_to_char(c));

	    if( (i+1) % B_SQ_SIZE == 0)
		putchar('|');
	    else
		putchar(' ');
	}

	putchar('\n');

	if( (j+1) % B_SQ_SIZE == 0)
	    print_line();
    }

}

void ss_print_ssquare_html(grid_t * g, int x, int y)
{
    /* existe t-il deja dans son sous carré ? */
    int square_x = x * B_SQ_SIZE;
    int square_y = y * B_SQ_SIZE;

    int i, j;

    puts("\t\t\t<table class=\"ss_sq_tbl\" width=\"100%\">");

    for(j = 0; j < B_SQ_SIZE; ++j)
    {
	puts("\t\t\t\t<tr>");
	for(i = 0; i < B_SQ_SIZE; ++i)
	{
	    box_t c = ss_get_box(g, square_x + i, square_y + j);

	    if(ss_get_known(g, square_x + i, square_y + j))
		printf("<td><b>%c</b></td>", ss_box_to_char(c));
	    else
		printf("<td>%c</td>", ss_box_to_char(c));

	}
        puts("\t\t\t\t</tr>");
    }

    puts("</table>");
}

void ss_print_grid_html(grid_t * g)
{

    int i, j;

    puts("<table class=\"ss_tbl\">");

    for(j = 0; j < B_SQ_SIZE; ++j)
    {
	puts("\t<tr>");
	for(i = 0; i < B_SQ_SIZE; ++i)
	{
	    puts("\t\t<td>");
	    ss_print_ssquare_html(g, i, j);
	    puts("\t\t</td>");
	}
	puts("\t</tr>");
    }

    puts("</table>");

}

void ss_goto_first_box(grid_t * g)
{
    g->pos.x = 0;
    g->pos.y = 0;
}

void ss_goto_bfirst_box(grid_t * g)
{
    g->pos.x = -1;
    g->pos.y = 0;
}

int ss_goto_next_box(grid_t * g)
{
    g->pos.x++;

    if(g->pos.x == B_SIZE)
    {
	g->pos.x = 0;
	g->pos.y++;
	if(g->pos.y == B_SIZE)
	    return 0;
    }
    return 1;
}

int ss_backto_previous_box(grid_t * g)
{

    g->pos.x--;

    if(g->pos.x == -1)
    {
	g->pos.x = B_SIZE - 1;
	g->pos.y--;
	if(g->pos.y == -1)
	    return 0;
    }
    return 1;
}

int ss_is_a_valid_possibility(grid_t * g, box_t v)
{
    int i, j;

    /* existe t-il deja sur la ligne ? */
    for(j = 0; j < B_SIZE; ++j)
	if(ss_get_box(g, g->pos.x, j) == v)
	    return 0;

    /* existe t-il deja sur la colonne ? */
    for(i = 0; i < B_SIZE; ++i)
	if(ss_get_box(g, i, g->pos.y) == v)
	    return 0;

    /* existe t-il deja dans son sous carré ? */
    int square_x = (int)(g->pos.x / B_SQ_SIZE) * B_SQ_SIZE;
    int square_y = (int)(g->pos.y / B_SQ_SIZE) * B_SQ_SIZE;

    for(j = 0; j < B_SQ_SIZE; ++j)
	for(i = 0; i < B_SQ_SIZE; ++i)
	    if(ss_get_box(g, square_x + i, square_y + j) == v)
		return 0; 

    return 1;
    
}

int ss_solve_grid(grid_t * g)
{
    ss_goto_first_box(g);
    return ss_solve_grid_next(g);
}

void dbg(char*s){fputs(s,stderr);}

int ss_solve_grid_next(grid_t * g)
{


    if(ss_get_known_c(g, g->pos))    
    { /* case connue */
	if(ss_goto_next_box(g))
	{
	    if(ss_solve_grid_next(g))
		return 1;
	    
	    ss_backto_previous_box(g);
	} /* on est a la fin*/
	else
	    return 1;  /* c'est gagné ! */

    }
    else /* case inconnue */
    {

	box_t c = ss_get_box_c(g, g->pos);

	/* pour chaque chiffre possible */
	int t;

	for(t = 1; t <= B_SIZE; ++t)
	{
	    if(ss_is_a_valid_possibility(g, t))
	    { /* on essaye */
		ss_get_box_c(g, g->pos) = t;

		/* et on va au suivant */

		if(ss_goto_next_box(g))
		{
		    if(ss_solve_grid_next(g))
			return 1;

		    ss_backto_previous_box(g);
		    ss_get_box_c(g, g->pos) = c;
		} /* on est a la fin*/
		else
		    return 1;  /* c'est gagné ! */
	    }

	}

    }


    return 0;
}

int ss_check_grid(grid_t * g)
{

    ss_goto_bfirst_box(g);

    while(ss_goto_next_box(g))
    {
	box_t c = ss_get_box_c(g, g->pos);

	if(c != B_UNKNOWN)
	{
	    ss_get_box_c(g, g->pos) = B_UNKNOWN;

	    int r = ss_is_a_valid_possibility(g, c);

	    ss_get_box_c(g, g->pos) = c;

	    if(!r)
		return 0;
	}
    }

    return 1;

}



