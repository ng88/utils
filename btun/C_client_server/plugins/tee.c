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

#include "common.h"
#include <stdio.h>
#include <string.h>
#include "../bool.h"

#define OUT_PF "> "
#define INP_PF "< "
#define PF_LEN 2

typedef enum
{
    TM_BINARY, TM_ASCII
} tee_mode_t;

typedef struct
{
    /** output file */
    FILE * out;
    
    /** options */
    tee_mode_t mode;


    int inp_line;
    int out_line;
    
} tee_t;

int bt_plugin_init(plugin_info_t * p)
{
    p->name = "tee";
    p->desc = "dump tunnel traffic to disk";
    p->author = "ng";
    p->version = 1;

    tee_t * st = (tee_t*)malloc(sizeof(tee_t));
    if(!st)
	return 0;

    st->out = p->argc == 0 ? stdout : fopen(p->argv[0], "wb");
    if(!st->out)
    {
	fprintf(stderr, "tee: unable to open `%s'!\n", p->argc == 0 ? "stdout" : p->argv[0]);
	return 0;
    }

    if(p->argc > 1 && !strcmp(p->argv[1], "ascii"))
	st->mode = TM_ASCII;
    else
	st->mode = TM_BINARY;

    st->inp_line = st->out_line = 0;

    p->data = st;

   /* init success */
    return 1;
}

void bt_plugin_destroy(plugin_info_t * p)
{
    tee_t * st = (tee_t*)p->data;
    fclose(st->out);
    free(st);
}

void dump_buffer(tee_t * st, char * in, size_t s, bool input)
{
   if(st->mode == TM_BINARY)
	fwrite(in, 1, s, st->out);
    else
    {
	const char * prefix;
	int * line;

	if(input)
	{
	    prefix = INP_PF;
	    line = &st->inp_line;
	}
	else
	{
	    prefix = OUT_PF;
	    line = &st->out_line;
	}

	fprintf(st->out, "@@ %d, %d, %d @@\n", s, st->inp_line, st->out_line);
	fwrite(prefix, 1, PF_LEN, st->out);
	size_t i;
	for(i = 0; i < s; ++i)
	    if(in[i] != '\r')
	    {
		fputc(in[i], st->out);
		if(in[i] == '\n')
		{
		    (*line)++;
		    if(i < s - 1)
			fwrite(prefix, 1, PF_LEN, st->out);
		}
	    }
    }

}


size_t bt_plugin_encode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    tee_t * st = (tee_t*)p->data;

   /* dump input traffic */
    dump_buffer(st, in, s, true);
 
    *out = in;
    return s;
}


size_t bt_plugin_decode(plugin_info_t * p, char * in, size_t s, char ** out)
{

    tee_t * st = (tee_t*)p->data;

   /* dump output traffic */
    dump_buffer(st, in, s, false);
    
    *out = in;
    return s;
}


