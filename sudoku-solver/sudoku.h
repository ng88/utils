

#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>

#define B_UNKNOWN 50

typedef unsigned char box_t;

typedef struct
{
    int x, y;
} coord_t;

typedef struct
{
    box_t boxes[9][9];
    coord_t pos;
    int success;

} grid_t;



#define ss_get_box(g, i, j) ((g)->boxes[(i)][(j)])
#define ss_get_box_c(g, c) ss_get_box((g), (c).x, (c).y)

int ss_read_grid_from_file(FILE * f, grid_t * dest);

void ss_print_grid(grid_t * g);


void ss_solve_grid(grid_t * g);
void ss_solve_grid_next(grid_t * g);

void ss_goto_next_box(grid_t * g);
void ss_backto_previous_box(grid_t * g);

/** test if v is a valid possibility for current position */
int ss_is_a_valid_possibility(grid_t * g, box_t v);

#endif
