#include <stdio.h>

#define CR 13
#define LF 10

void usage(char * str)
{
    fprintf(stderr, "utilisation: %s fichier_entree fichier_sortie\n", str);
    exit(1);
}

int main(int argc, char ** argv)
{
    int c;
    FILE *in, *out;

    in = stdin; out = stdout;

    while( (c = fgetc(in)) != EOF )
    {
        if(c == CR)  /* on supprime les CR */
            fputc(LF, out);
        else
            fputc(c, out);
    }





}
