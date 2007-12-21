/***************************************************************************
 *   This file is part of the 'utils' projects                             *
 *                                                                         *
 *   'utils' projects                                                      *
 *                                                                         *
 *   Copyright (C) 2006, 2008 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://www.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/

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


/** test if (i, j) is a known box (ie gived) */
#define ss_get_known(g, i, j) ((g)->known[(i)][(j)])
/** same as above
  * /!\ multiple c evaluation
  */
#define ss_get_known_c(g, c) ss_get_known((g), (c).x, (c).y)

/** return the box value, can be [1, B_SIZE] or B_UNKNOWN */
#define ss_get_box(g, i, j) ((g)->boxes[(i)][(j)])
/** same as above
  * /!\ multiple c evaluation
  */
#define ss_get_box_c(g, c) ss_get_box((g), (c).x, (c).y)

/** convert a box value to something readable
  * /!\ multiple c evaluation 
  */
#define ss_box_to_char(c) ((c) == B_UNKNOWN ? '.' : '0' + (c))

/** Load the grid in file f into dest.
  * File format must be something like this:
  * 
  * 75___68__
  * 9_25__4__
  * _4____67_
  * _1__8__6_
  * ___2__5_1
  * 4____1__9
  * 139_5____
  * __63____7
  * ____92_38
  *
  */
int ss_read_grid_from_file(FILE * f, grid_t * dest);

/** Print g to stdout (plain text) */
void ss_print_grid_text(grid_t * g);

/** Print g to stdout (html) */
void ss_print_grid_html(grid_t * g);

/** Check is g is valid */
int ss_check_grid(grid_t * g);

/** Solve g if possible (non-zero is returned on success) */
int ss_solve_grid(grid_t * g);
int ss_solve_grid_next(grid_t * g);

/** goto (0, 0) */
void ss_goto_first_box(grid_t * g);
/** goto the box before (0, 0), useful whith ss_goto_next_box() */
void ss_goto_bfirst_box(grid_t * g);
/** goto next box */
int ss_goto_next_box(grid_t * g);
/** goto previous box */
int ss_backto_previous_box(grid_t * g);

/** test if v is a valid possibility for current position */
int ss_is_a_valid_possibility(grid_t * g, box_t v);

#endif
