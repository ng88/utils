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

#ifndef CRYPTOR_H
#define CRYPTOR_H

#include "user.h"

typedef enum
{
    C_NONE,
    C_XOROR
} c_mode_t;


void set_encryptor_mode(c_mode_t m);

void reinit_encryptor(user_t * u);
char * encrypt(char * src, char * dst);


#define free_encryptor() set_encryptor_mode(C_NONE)



#endif
