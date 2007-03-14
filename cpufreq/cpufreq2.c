
/**
  * Author: Nicolas GUILLAUME <ng@ngsoft-fr.com>
  *
  * Compile with :
  *      gcc -D_REENTRANT cpufreq2.c -lpthread -Wall
  */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>

typedef unsigned int uint;

/** Flag that indicates if the computing_thread should countinue to work */
volatile int bwork;

/** Return the value of the Time Stamp Counter */
inline uint64_t timestamp()
{
#ifdef __i386
    uint64_t ts;
    asm("rdtsc" : "=A" (ts));
    return ts;
#elif __amd64
    uint64_t a, d;
    asm("rdtsc" : "=a" (a), "=d" (d));
    return (d<<32) | a;
#endif
}

/** Thread that generates some CPU activity */
void * computing_thread(void * arg)
{
    int i = 0, j;
    while(bwork)
    {
        i += 5;
        j = i / 45;
        j = i / ( (j * j) ? -j : j*j+2) - 4 * i;
        i %= 874;
    }
    pthread_exit(0);
}

/** Return the CPU frequency (computed during ms msec) */
uint compute_cpu_freq_ms(uint ms)
{
    pthread_t twork;
    bwork = 1;
    
    if(pthread_create (&twork, NULL, computing_thread, "1") < 0)
        return 0;
            
    uint64_t start = timestamp();
    
    if(usleep(ms * 1000))
        return 0;

    uint ret = (uint)( (timestamp() - start) / ms * 1000 );
    
    bwork = 0;

    pthread_join(twork, NULL);
            
    return ret;
}

/** Return the CPU frequency (computed during 0.1 sec) */
inline uint compute_cpu_freq()
{
    return compute_cpu_freq_ms(100);
}


int main(int argc, char ** argv)
{
    uint ms = 0;
    
    if(argc > 1)
        ms = atoi(argv[1]);
       
    uint freq = (ms > 0) ? compute_cpu_freq_ms(ms) : compute_cpu_freq();

    printf("%d Mhz\n", freq / 1000000);

    return 0;
}
