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

#include "cryptor.h"
#include "../xoror/xoror.h"
#include "assert.h"

#define XOROR_KEY 0xfeab

c_mode_t cmode = C_XOROR;
cryptor * ec = NULL;


void set_encryptor_mode(c_mode_t m)
{
    cmode = m;

    switch(cmode)
    {
    case C_XOROR:
	if(!ec) ec = cryptor_malloc();
	break;
    case C_NONE:
	if(ec) free(ec);
	break;
    }

}

void reinit_encryptor(user_t * u)
{
    c_assert(u);

    switch(cmode)
    {
    case C_XOROR:
	c_assert(ec);
	cryptor_init(ec, u->passphrase, XOROR_KEY);
    case C_NONE:
	break;
    }
}

char * encrypt(char * src, char * dst)
{
    c_assert(src);

    switch(cmode)
    {
    case C_XOROR:
	c_assert(ec);
	encrypt_string(src, dst, ec);
	return dst;
    case C_NONE:
	return src;
    }

    return NULL;
}


