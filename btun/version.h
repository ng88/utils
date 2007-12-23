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

#ifndef VERSION_H
#define VERSION_H

#define CLIENT_NAME      "btun"
#define CLIENT_VERSION   "0.2"
#define CLIENT_REVISION  __SVN_HEAD__
#define CLIENT_DATE      __DATE__

#define SERVER_NAME      "btund"
#define SERVER_VERSION   "0.2"
#define SERVER_REVISION  __SVN_HEAD__
#define SERVER_DATE      __DATE__

#define PROTOCOL_VERSION "0.1"


#define SC_COPYRIGHT  "Copyright (C) 2006, 2008 by Nicolas GUILLAUME. All rights reserved.\n" \
                      "ng@ngsoft-fr.com\n\n"                                                  \
                      "http://svn.ngsoft-fr.com/trac/utils/\n\n"                              \
                      "This program is free software; you can redistribute it and/or modify\n"\
                      "it under the terms of the GNU General Public License as published by\n"\
                      "the Free Software Foundation; version 2 of the License only.\n"        \
                      "See the COPYING file.\n\n"                                             \
                      "Using MD5 hash algorithm:\n"                                           \
                      "Copyright (C) 1990, RSA Data Security, Inc. All rights reserved.\n"    



#ifndef __DATE__
#define __DATE__ "<unknow>"
#endif

#ifndef __SVN_HEAD__
#define __SVN_HEAD__ "<unknow>"
#endif

#endif

