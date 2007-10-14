

#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>

#define B_UNKNOWN 50

#define B_SIZE 9
#define B_SQ_SIZE 3

typedef unsigned char box_t;

typedef struct
{
    int x, y;
} coord_t;

typedef struct
{
    box_t boxes[B_SIZE][B_SIZE];
    char known[B_SIZE][B_SIZE];
    coord_t pos;

} grid_t;


#define ss_get_known(g, i, j) ((g)->known[(i)][(j)])
/** /!\ multiple c evaluation */
#define ss_get_known_c(g, c) ss_get_known((g), (c).x, (c).y)
#define ss_get_box(g, i, j) ((g)->boxes[(i)][(j)])
/** /!\ multiple c evaluation */
#define ss_get_box_c(g, c) ss_get_box((g), (c).x, (c).y)
/** /!\ multiple c evaluation */
#define ss_box_to_char(c) ((c) == B_UNKNOWN ? '.' : '0' + (c))

int ss_read_grid_from_file(FILE * f, grid_t * dest);

void ss_print_grid(grid_t * g);

int ss_check_grid(grid_t * g);

int ss_solve_grid(grid_t * g);
int ss_solve_grid_next(grid_t * g);

void ss_goto_first_box(grid_t * g);
void ss_goto_bfirst_box(grid_t * g);
int ss_goto_next_box(grid_t * g);
int ss_backto_previous_box(grid_t * g);

/** test if v is a valid possibility for current position */
int ss_is_a_valid_possibility(grid_t * g, box_t v);

#endif
