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
#include <time.h>
#include <stdlib.h>

#define  CLEN_SIZE 16
#define  CACHE_PREVENT_SIZE 16

/* This plugin simulates an HTTP communication.
 */


static char client_request[] = 
             "POST %s%d HTTP/1.0\r\n"
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
    ST_WAIT_CONTENT_LEN,
    ST_WAIT_HEADER_END,
    ST_WAIT_PAYLOAD_END,
} state_t;

typedef struct
{
    size_t size;
    char * header;
    char * agent;
    char * host;
    char * url;
    char mode_client;

    state_t state;
    size_t remain;
} hd_t;


int bt_plugin_init(plugin_info_t * p)
{
    srand(time(0));

    p->name = "http";
    p->desc = "sham HTTP";
    p->author = "ng";
    p->version = 1;

    hd_t * hd = malloc(sizeof(hd_t));
    p->data = hd;
    c_assert(hd);

    hd->host = "www.anyhost.com";
    hd->agent = "BTun HTTP plugin";
    hd->url = "/index.html?cp=";

    hd->mode_client = 1;
    hd->state = ST_WAIT_CONTENT_LEN;
    hd->remain = 0;

    if(p->argc > 0 && !strcmp(p->argv[0], "server"))
	hd->mode_client = 0;

    if(hd->mode_client)
    {
	if(p->argc > 1) hd->host = p->argv[1];
	if(p->argc > 2) hd->agent = p->argv[2];
	if(p->argc > 3) hd->url = p->argv[3];

	hd->size = sizeof(client_request) + 2
	    + strlen(hd->host) + strlen(hd->agent) + strlen(hd->url) + CLEN_SIZE + CACHE_PREVENT_SIZE;
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

    ensure_buffer_size(p, s + hd->size + 2);
    if(!p->buffer)
	return BT_ERROR;

    if(hd->mode_client)
	snprintf(p->buffer, hd->size + 1, client_request, hd->url, rand(), hd->host, hd->agent, s);
    else
	snprintf(p->buffer, hd->size + 1, server_answer, hd->agent, s);

    size_t strsize = strlen(p->buffer);

    memcpy(p->buffer + strsize, in, s);

    *out = p->buffer;

    return s + strsize;
}


/** buggy, need packet defragmentation, by using a 'Content_Length:' field for example 
attentions, les proxy peuvent modifier les entetes, on ne doit pas présumer l'ordre, ni la casse
init:
 attends Content-Length:  %u
 attends \r\n\r\n
 on compte la bonne quantité de données, ce qui permets de gérer la fragmentation ainsi que du HTTP over HTTP sans pb
goto init
gerer tout ca dans un automate comme c'est deja le cas


    ST_WAIT_CONTENT_LEN,
    ST_WAIT_HEADER_END,
    ST_WAIT_PAYLOAD_END,

*/
size_t bt_plugin_decode(plugin_info_t * p, char * in, size_t s, char ** out)
{
    /* Here we have to remove the HTTP header
       in order to get our data back.
    */

    char * current_buff = in;
    size_t current_buff_size = s;
    char * out_buff = in;
    size_t out_buff_size = 0;
    hd_t * hd = (hd_t*)p->data;

   /*  ensure_buffer_size(p, s);
    if(!p->buffer)
	return BT_ERROR;
   */
   
   if(hd->state == ST_WAIT_CONTENT_LEN)
   {
       char * r = strstr(current_buff, "\r\nContent-Length: ");
       if(r)
       {
	   unsigned int cl = 0;
	   if(sscanf(r + 2, "Content-Length: %u\r\n", &cl))
	   {
	       hd->remain = cl;
	       hd->state = ST_WAIT_HEADER_END;
	   }

       }
   }

   if(hd->state == ST_WAIT_HEADER_END)
   {
       char * r = strstr(current_buff, "\r\n\r\n");
       if(r)
       {
	   current_buff = r + 4;
	   current_buff_size = current_buff_size - (r - current_buff) - 4;
	   hd->state = ST_WAIT_HEADER_END;
       }
   }

   if(hd->state == ST_WAIT_PAYLOAD_END)
   {
       if(current_buff_size > hd->remain)
	   return BT_ERROR; /* many packet in one not supported for the moment */
       else
       {
	   hd->remain -= current_buff_size;
	   out_buff = current_buff;
	   out_buff_size = current_buff_size;
       }
   }

   *out = out_buff;
   return out_buff_size;
}


