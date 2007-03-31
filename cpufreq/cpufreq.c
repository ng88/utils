/***************************************************************************
 *   This file is part of the 'utils' projects                             *
 *                                                                         *
 *   'utils' projects                                                      *
 *                                                                         *
 *   Copyright (C) 2006, 2007 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://www.ngsoft-fr.com/                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/                                                                


/**
  * Author: Nicolas GUILLAUME <ng@ngsoft-fr.com>
  */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

typedef unsigned int uint;

/** Return the value of the Time Stamp Counter */
inline uint64_t timestamp()
{
    uint64_t ts;
    asm("rdtsc" : "=A" (ts));
    return ts;
}

/** Return the CPU frequency (computed during ms msec) */
uint compute_cpu_freq_ms(uint ms)
{
    uint64_t start = timestamp();
    
    if(usleep(ms * 1000))
        return 0;

    return (uint)( (timestamp() - start) / ms * 1000 );
}

/** Return the CPU frequency (computed during 1 sec) */
inline uint compute_cpu_freq()
{
    return compute_cpu_freq_ms(1000);
}


int main(int argc, char ** argv)
{
    uint ms = 0;
    
    if(argc > 1)
        ms = atoi(argv[1]);
       
    uint freq = (ms >= 100) ? compute_cpu_freq_ms(ms) : compute_cpu_freq();

    printf("%d Mhz\n", freq / 1000000);

    return 0;
}
