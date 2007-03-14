

#include <stdio.h>
#include <stdlib.h>

/*
    range 1 ... 10
    range 5
*/

void usage(char * str)
{
    fprintf(stderr, "usage: %s N ... M\n"
                    "       %s N\n", str, str, str);
    exit(1);
}

int main(int argc, char ** argv)
{
    int i, max, min, step;
    char * exename = strrchr(argv[0], '/');
    
    if( !exename )
        exename = argv[0];
     else
         exename++;

    if( argc >= 2 )
    {
        if( argc == 4 && strcmp(argv[2], "...") == 0 )
        {
            min = atoi(argv[1]);
            max = atoi(argv[3]);
        }
        else if( argc == 2 )
        {
            max = atoi(argv[1]);
            min = (max > 0) ? 1 : -1;
        }
        else
            usage(exename);
            
        step = (max < min) ? -1 : 1;
        max += step;
            
        for( i = min; i != max; i += step)
            printf("%d ", i);
            
        putchar('\n');
    }
    else
        usage(exename);
    
    return 0;
}
