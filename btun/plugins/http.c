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
#include <ctype.h>
#include <string.h>

#define  CLEN_SIZE 16

/* This plugin simulates an HTTP communication.
 */


static char client_request[] = 
             "GET /index.html HTTP/1.0\r\n"
             "Host: %s\r\n"
             "Connection: Keep-Alive\r\n"
             "User-Agent: %s\r\n"
             "Content-Length: %u\r\n"
             "\r\n";

static char server_answer[] = 
             "HTTP/1.1 200 OK\r\n"
             "Server: %s\r\n"
             "Content-Type: text/html\r\n"
             "Connection: Keep-Alive\r\n"
             "Content-Length: %u\r\n"
             "\r\n";

typedef enum
{
    ST_WAIT_HEADER,
    ST_WAIT_DATA,
} state_t;

typedef struct
{
    size_t size;
    char * header;
    char * agent;
    char * host;
    char mode_client;

    state_t state;
    size_t remain;
} hd_t;


int bt_plugin_init(plugin_info_t * p)
{

    p->name = "http";
    p->desc = "sham HTTP";
    p->author = "ng";
    p->version = 1;

    hd_t * hd = malloc(sizeof(hd_t));
    p->data = hd;
    c_assert(hd);

    hd->host = "www.anyhost.com";
    hd->agent = "BTun HTTP plugin";
    hd->mode_client = 1;
    hd->state = ST_WAIT_HEADER;
    hd->remain = 0;

    if(p->argc > 0 && !strcmp(p->argv[0], "server"))
	hd->mode_client = 0;

    if(hd->mode_client)
    {
	if(p->argc > 1) hd->host = p->argv[1];
	if(p->argc > 2) hd->agent = p->argv[2];

	hd->size = sizeof(client_request) + 2
	    + strlen(hd->host) + strlen(hd->agent) + CLEN_SIZE;
    }
    else
    {
	if(p->argc > 1) hd->agent = p->argv[1];

	hd->size = sizeof(client_request) + 2
	    + strlen(hd->agent) + CLEN_SIZE;

    }
    
    hd->header = (char*)malloc(hd->size);
    c_assert(hd->header);

    return 1;
}

void bt_plugin_destroy(plugin_info_t * p)
{
    if(p->buffer)
	free(p->buffer);

    free( ((hd_t*)p->data)->header );
    free(p->data);
}


size_t bt_plugin_encode(plugin_info_t * p, char * in, size_t s, char ** out)
{

   /* Here we add the HTTP header.
   */


    hd_t * hd = (hd_t*)p->data;

    ensure_buffer_size(p, s + hd->size);
    if(!p->buffer)
	return BT_ERROR;

    if(hd->mode_client)
	snprintf(p->buffer, hd->size, client_request, hd->host, hd->agent, s);
    else
	snprintf(p->buffer, hd->size, server_answer, hd->host, s);

    size_t strsize = strlen(p->buffer);

     strncpy(p->buffer + strsize, in, s);

    *out = p->buffer;

    return s + strsize;
}


/** buggy, need packet defragmentation, by using a 'Content_Length:' field for example */
size_t bt_plugin_decode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    /* Here we have to remove the HTTP header
       in order to get older data back.
    */

   hd_t * hd = (hd_t*)p->data;

    ensure_buffer_size(p, s + hd->size);
    if(!p->buffer)
	return BT_ERROR;

    if(hd->state == ST_WAIT_HEADER)
    {


	/* we look for the first \r\n\r\n sequence */

	size_t i;
	for(i = 0; i < s - 3; ++i)
	{
	    if(in[i]     == '\r' && 
	       in[i + 1] == '\n' &&
	       in[i + 2] == '\r' &&
	       in[i + 3] == '\n')
	    {
		*out = in + i + 4;
		return s - i - 4;
	    }
	}

	/* data corrupted or not from our plugin */
	return BT_ERROR;
    }

    if(hd->state == ST_WAIT_DATA)
    {
    }


}


