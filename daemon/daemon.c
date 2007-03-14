#include <unistd.h>
#include <stdio.h>

void usage(char * app)
{
    fprintf(stderr, "usage: %s command\n", app);
    exit(1);
}

int main(int argc, char ** argv)
{
    char * exename = strrchr(argv[0], '/');
    
    if( !exename )
        exename = argv[0];
     else
         exename++;
         
    if( argc != 2 ) usage(exename);
    
    if( daemon(1, 1) != 0 )
    {
        fprintf(stderr, "%s: daemon() call error\n", exename);
        exit(1);
    }
    
    system(argv[1]);
    
    
    return 0;
}
