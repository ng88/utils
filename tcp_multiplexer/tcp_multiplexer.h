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

#ifndef TCP_MUX_H
#define TCP_MUX_H

#include <stdlib.h>
#include <netinet/in.h>
#include "common.h"

#include "tcp_connection.h"


/**

      L'idée est d'implémenter un multpilexeur TCP

      C'est à dire un genre de nc capable de transmettre plusieurs
      connexions à travers l'entrée et la sortie standard


      Exemple d'utilisation désirée :

          client http

              /\
              ||
              \/


      tcp_multiplexer 80 # ecoute sur le port 80
                
              /\
             /||\
              ||

         stdin/stdout # pourra etre remplacé par un tunnel btun

              ||
             \||/
              \/

      tcp_multiplexer localhost 80 # connecte le port 80 sur localhost

              /\
              ||
              \/

             httpd


      Cela permettra de faire des tunnels TCP avec btun (entre autre)
      Car nc ne fait passer qu'une connexion à la fois.


*/

/** Packet type */
enum
{
    /** New connection/channel */
    RT_CONNECT = 0,

    /** Close connection/channel */
    RT_CLOSE = 1,

    /** Send/recv data */
    RT_DATA = 2,
};


/** Magic number */
#define RT_MAGIC ((char)0xAA)

enum { RT_BUFF = 600 };

/** Packet header */
typedef struct
{
    /** Magic number to ensure minimal consistency */
    char magic;

    /** Packet type (new connection, ...) */
    char type;

    /** Channel ID */
    uint16_t id;

    /** Data len (if any) */
    uint16_t len;
} header_t;

#define REQ_HEADER_SIZE (sizeof(char) * 2 + sizeof(uint16_t) * 2)

int send_request(tcp_connection_t * e, int fdout, unsigned char type, uint16_t data_len, char * sdata);
int recv_request(int fd);

void set_addr(char * addr, port_t port);
int tcp_mux(int fdin, int fdout);


void stop();

#endif
