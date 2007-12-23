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

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <netinet/in.h>
typedef uint16_t port_t;

#define SERVER_DEFAULT_PORT 12358

#define USER_MAX_PASS_SIZE 64
#define USER_MAX_LOGIN_SIZE 8
#define USER_MAX_CHANNEL_SIZE 16

#define CHALLENGE_SIZE 16


/* channel options */
enum
{
    OPT_MASTER = 1,
    OPT_RESTRICTED = 2,
};

typedef unsigned char option_t;

/* user access  */
enum
{
    UA_GRANTED = 0,
    UA_DENIED = 1,
};

/* channel access  */
enum
{
    CA_GRANTED = 0,
    CA_DENIED = 1,
    CA_CANT_BE_MASTER = 2,
    CA_TOO_MUCH_CHANNEL = 3,
    CA_TOO_MUCH_USER = 4,
    CA_CANT_CHPERM = 5,
};


#endif
