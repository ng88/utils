/******************************************************************************
 *   btun/btund is a powerful backward tunnel creator                         *
 *                                                                            *
 *   An user guide is available on:                                           *
 *    http://svn.ngsoft-fr.com/trac/utils/wiki/BtunUserGuideEnglish           *
 *                                                                            *
 ******************************************************************************/
 
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

#include "user.h"

#include <string.h>

#include "assert.h"

user_pool_t * create_user_pool()
{
    user_pool_t * r = 
	(user_pool_t *)malloc(sizeof(user_pool_t));

    c_assert2(r, "malloc failed");

    r->users = create_vector(8);

    return r;
}

bool read_delim_string(char* s, int max, char sep, FILE * f)
{
    c_assert(s && f);

    int i;
    int c;
    for(i = 0; i < max; ++i)
    {
	if( (c = fgetc(f)) == EOF)
	    return false;

	s[i] = (char)c;

	if(s[i] == sep)
	{
	    s[i] = '\0';
	    return true;
	}
    }

    if((c = fgetc(f)) == EOF)
	return false;

    return ((char)c == sep);
}

void read_users_from_file(user_pool_t * p, FILE * f)
{
    c_assert(p && f);

    enum { BSIZE = 2 };
    char login[USER_MAX_LOGIN_SIZE + 1];
    char pass[USER_MAX_PASS_SIZE + 1];
    char buff[BSIZE];

    int c;

    while(!feof(f))
    {
	c = fgetc(f);

	if(c == EOF) return;

	switch((char)c)
	{
	case '\n':
	    break;
	case '#':
	    while(c != EOF && (char)c != '\n')
		c = fgetc(f);
	    break;
	case ':':

	    if(!read_delim_string(login, USER_MAX_LOGIN_SIZE + 1, ':', f))
		return;

	    if(!read_delim_string(pass, USER_MAX_PASS_SIZE + 1, ':', f))
		return;

	    if(!read_delim_string(buff, BSIZE + 1, ':', f))
		return;

	    if(!read_delim_string(buff, BSIZE + 1, '\n', f))
		return;

	    dbg_printf("add user %s\n", login);
	    user_add(p, create_user(login, pass));

	    break;
	default:
	    dbg_printf("syntax error in users configuration file\n");
	}

    }
}

void reload_users_from_file(user_pool_t * p, char * fn)
{
    FILE * f = fopen(fn, "r");

    if(!f)
	return;

    clear_user_pool(p);
    read_users_from_file(p, f);

    fclose(f);
}

user_t * get_user_from_name(user_pool_t * p, char * login)
{
    c_assert(p && login);

    size_t s = user_count(p);
    size_t i;

    for(i = 0; i < s; ++i)
    {
	user_t * r = get_user_at(p, i);
	if(!strcmp(r->login, login))
	    return r;
    }
    return NULL;
}

void print_user_pool(user_pool_t * p, FILE * f)
{
    c_assert(p && f);

    size_t s = user_count(p);
    size_t i;

    fprintf(f, "---- user table --------------\n");

    for(i = 0; i < s; ++i)
	print_user(get_user_at(p, i), f);

    fprintf(f, "---- %3u users ---------------\n", s);

}

void clear_user_pool(user_pool_t * p)
{
    c_assert(p);

    size_t s = user_count(p);
    size_t i;

    for(i = 0; i < s; ++i)
	free_user(get_user_at(p, i));

    vector_clear(p->users, 0);
}

void free_user_pool(user_pool_t * p)
{
    c_assert(p);

    clear_user_pool(p);

    free_vector(p->users, 0);

    free(p);
}



user_t * create_user(char * login, char * pass)
{
    c_assert(login && pass);

    user_t * r = 
	(user_t *)malloc(sizeof(user_t));

    c_assert2(r, "malloc failed");

    r->login = strdup(login);
    r->passphrase = strdup(pass);

    return r;
}

void print_user(user_t * u, FILE * f)
{
    c_assert(u && f);

    fprintf(f, "%s\n", u->login);
}

void free_user(user_t * u)
{
    c_assert(u);

    free(u->login);
    free(u->passphrase);

    free(u);
}



