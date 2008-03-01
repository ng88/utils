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

/**

      L'idée est d'avoir d'implémenter un multpilexeur TCP

      C'est à dire un genre de nc capable de transmettre plusieurs
      connexions à travers l'entrée et la sortie standard


      Exemple d'utilisation désirée :

        client http / ssh

              /\
              ||
              \/


      tcp_multiplexer -l 80 -l 22 # ecoute sur les ports 22/80
                
              /\
             /||\
              ||

         stdin/stdout # pourra etre remplacé par un tunnel btun

              ||
             \||/
              \/

      tcp_multiplexer localhost # connecte les ports 22/80 sur localhost

              /\
              ||
              \/

         httpd / sshd


      Cela permettra de faire des tunnels TCP avec btun (entre autre)
      Car nc ne fait passer qu'une connexion à la fois.


*/

enum
{
    RT_CONNECT = 0,
    RT_CLOSE = 1,
    RT_DATA = 2,
};

#define RT_MAGIC ((char)0xAA)

enum { RT_BUFF = 512, RT_SAFE_SIZE = 500 };

#define REQ_HEADER_SIZE (sizeof(uint16_t) * 2 + sizeof(unsigned char) * 2)

int send_request(int fd, unsigned char type, uint16_t id, uint16_t data_len, char * sdata);
int recv_request(int fd);
int start_tcp_tunnel_server(port_t port);



#endif
