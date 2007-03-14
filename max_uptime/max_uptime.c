#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Author: Nicolas GUILLAUME <ng@ngsoft-fr.com>
 *
 */

#define PROC_UPTIME "/proc/uptime"
#define VERSION 0.1

typedef struct
{
    double uptime;
    double idletime;
} uptime_t;


void show_uptime(char * txt, uptime_t infos)
{
    long time = (long)infos.uptime;
    
    if(time == -1)
        printf("%s: not defined\n", txt);
    else
        printf("%s: %ld days, %ld:%02ld:%02ld\n", txt, time / (3600*24), 
            (time % (3600*24)) / 3600, (time % 3600) / 60, time % 60);
}

/*-s show max only
*/

uptime_t get_uptime(char * file)
{
    uptime_t infos;
    infos.idletime = infos.uptime = -1;
    
    FILE * fp = fopen(file, "r");
    
    if(!fp)
        return infos;
        
    fscanf(fp, "%lf %lf\n", &infos.uptime, &infos.idletime);
    fclose(fp);
    
    return infos;
}


void usage(char * exe)
{
    char * p = strrchr(exe, '/');
    if(p) exe = ++p;
    
    fputs(exe, stderr);
    fputs(": usage: ", stderr);
    fputs(exe, stderr);
    fputs(" [-s] path_to_max\n"
          "   -s           only show the maximum uptime\n"
          "   path_to_max  path where the maximum uptime is stored\n\n"
          "  This program, when called without -s, compares " PROC_UPTIME " and\n"
          "  path_to_max and stores the maximum uptime in path_to_max.\n\n"
          "  Note: path_to_max file  is created if it not exists.\n"
         , stderr);
}

int main(int argc, char ** argv)
{
    char * path_max;
    int show_only = 0;

    if(argc == 2 && strcmp(argv[1], "-s"))
        path_max = argv[1];
    else if(argc == 3 && !strcmp(argv[1], "-s"))
    {
        path_max = argv[2];
        show_only = 1;
    }
    else
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    if(show_only)
    {
        show_uptime("Max Uptime", get_uptime(path_max));
    }
    else
    {
        uptime_t max_uptime = get_uptime(path_max);
        uptime_t current_uptime = get_uptime(PROC_UPTIME);
        
        if(current_uptime.uptime == -1)
        {
            fputs("unable to open " PROC_UPTIME "\n", stderr);
            return EXIT_FAILURE;
        }
        
        if(current_uptime.uptime > max_uptime.uptime)
        {
            FILE * fp = fopen(path_max, "w");
            
            if(!fp)
            {
                fprintf(stderr, "unable to open %s for writing\n", path_max);
                return EXIT_FAILURE;
            }
            fprintf(fp, "%.2lf %.2lf\n", current_uptime.uptime, current_uptime.idletime);
            fclose(fp);

	    max_uptime = current_uptime;
        }

        show_uptime("Max Uptime", max_uptime);
        show_uptime("Current Uptime", current_uptime);
    }

   
    return EXIT_SUCCESS;
}

