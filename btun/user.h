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

#ifndef USER_H
#define USER_H

#include <stdio.h>

#include "vector.h"
#include "bool.h"

#include "protocol.h"

typedef struct
{
    int fd;
    char * login;
    char * passphrase;
    bool is_root;
} user_t;

typedef struct
{
    /* vector of user_t * */
    vector_t * users;
} user_pool_t;


user_pool_t * create_user_pool();
void read_users_from_file(user_pool_t * p, FILE * f);
user_t * get_user_from_name(user_pool_t * p, char * login);
#define get_user_at(p, i) \
        ((user_t *)vector_get_element_at((p)->users, (i)))
#define user_count(p) \
          (vector_size((p)->users))
#define user_add(p, u) \
         (vector_add_element((p)->users, (u)))
void print_user_pool(user_pool_t * p, FILE * f);
void free_user_pool(user_pool_t * p);


user_t * create_user(char * login, char * pass, bool is_root);
void print_user(user_t * u, FILE * f);
void free_user(user_t * u);


bool read_delim_string(char* s, int max, char sep, FILE * f);

#endif
