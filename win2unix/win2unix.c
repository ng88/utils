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
    
    char * exename = strrchr(argv[0], '/');
    
    if( !exename )
        exename = argv[0];
    else
        exename++;
        
    if(argc != 3)
        usage(exename);
         
         
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");

    if(!(in && out))
    {
        puts("Erreur d'entree/sortie - impossible d'ouvrir le fichier d'entree ou de sortie");
        usage(exename);
    }

    while( (c = fgetc(in)) != EOF )
        if(c != CR)  /* on supprime les CR */
            fputc(c, out);

    
    fclose(in);
    fclose(out);
        
    

}

