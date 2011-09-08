

#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "assert.h"

/** @author Nicolas GUILLAUME
 */

typedef signed long long int file_pos_t;
typedef union
{
    uint64_t u64;
    uint32_t u32[2];
} u62w_t;

typedef struct
{
    /** Signature content */
    char * sig;
    /** Signature len */
    size_t len;
    /** Signature offset */
    size_t offset;
    /** File extension */
    char * ext;
    /** Size to extract */
    size_t sz;
} signature_t;


/**
 * @param f opened dump file
 * @param matches destination vector for matches (entry type is file_pos*)
 * @param sig the signature to look for
 */
void search_signatures(FILE * dump, vector_t * matches, const signature_t * sig);

/**
 * @param dump pointer to the opened dump file
 * @param matches list of matches (entry type is file_pos*)
 * @param destfolder path to the destination folder
 * @param destcountstart value used to initialize the file counter used in the  extracted file name
 * @param sig the signature to look for
 */
void extract_files(FILE * dump, vector_t * matches, const char * destfolder, int destcountstart, const signature_t * sig);

enum
{
  KiB = 1024,
  MiB = 1024 * KiB,
  GiB = 1024 * MiB,
};

/** Signature count */
enum { SIGS_COUNT = 4 };
/** Signature list */
static const signature_t SIGS[SIGS_COUNT] =
{
  /* signature, taille de la signature, offset par rapport au début du fichier, extension du fichier, taille à extraire*/
  {"\xFF\xD8\xFF\xE0", 4, 0, "jpg", 8 * MiB},
  {"\xFF\xD8\xFF\xE1", 4, 0, "jpg", 8 * MiB},
  {"ftypqt", 6, 4, "mov", 64 * MiB},
  {"ftypM4V", 7, 4, "m4v", 64 * MiB},
};


int main(int argc, char * argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "usage: %s dump_file dest_folder\n"
                        "  dump_file   memory dump file\n"
                        "  dest_folder destination folder where file will be extracted, should be empty\n"
                , argv[0]);
        return 1;
    }

    int i;
    int matchcount = 0;
    vector_t * matches = create_vector(32);

    printf("Opening %s...\n", argv[1]);
    FILE * f = fopen(argv[1], "rb");
    c_assert(f);

    // for each signature
    for(i = 0; i < SIGS_COUNT; i++)
    {
        const signature_t * sig = &SIGS[i];

        printf("Looking for %s (%d)...\n", sig->ext, i);
        vector_clear(matches);
        search_signatures(f, matches, sig);
	extract_files(f, matches, argv[2], matchcount, sig);	
	matchcount += vector_size(matches);
    }
    
    free_vector(matches, 1);
    fclose(f);

    printf("Done, %d matches found.\n", matchcount);

    return 0;
}

inline file_pos_t * new_fp(file_pos_t p)
{
    file_pos_t * r = (file_pos_t*)malloc(sizeof(file_pos_t));
    c_assert(r);
    *r = p;
    return r;
}


void search_signatures(FILE * f, vector_t * dest, const signature_t * sig)
{
    enum { BSIZE = 65536 };
    char * buffer = malloc(BSIZE);
    size_t sz = 0;
    size_t i;
    size_t matchPos = 0;
    file_pos_t file_pos = 0;
    file_pos_t last_match_file_pos = -1;

    rewind(f);
    while(!feof(f))
    {
        sz = fread(buffer, 1, BSIZE, f);
        for(i = 0; i < sz; i++)
        {
            if(buffer[i] == sig->sig[matchPos])
            {
                matchPos++;
                if(matchPos == sig->len)
                {
                    matchPos = 0;
                    // absolute file position
                    last_match_file_pos = file_pos + i - sig->len - sig->offset + 1;
                    vector_add_element(dest, new_fp(last_match_file_pos));
                    u62w_t tmp;
                    tmp.u64 = last_match_file_pos;
                    printf("match found @0x%02X%08X\n", tmp.u32[1], tmp.u32[0]);
                }
            }
            else
            {
                matchPos = 0;
            }
        }
        file_pos += sz;
    }
    free(buffer);
}


void extract_files(FILE * dump, vector_t * matches, const char * destfolder, int destcountstart, const signature_t * sig)
{

    enum { BSIZE = 65536 };
    char * buffer = malloc(BSIZE);

    enum { MAX_PATH_LEN = 512 };
    char path[MAX_PATH_LEN];
    printf("Extracting files from dump in %s...\n", destfolder);
    size_t i;
    for(i = 0; i < vector_size(matches); i++)
    {
        file_pos_t * p = (file_pos_t*)vector_get_element_at(matches, i);

        u62w_t tmp;
        tmp.u64 = *p;

        snprintf(path, MAX_PATH_LEN, "%s/EF_%04d_%02X%08X.%s", destfolder, destcountstart + i, tmp.u32[1], tmp.u32[0], sig->ext);

        printf("Writing %s from 0x%02X%08X...  ", path, tmp.u32[1], tmp.u32[0]);
        fflush(stdout);

        FILE * dest = fopen(path, "wb");
        if(!dest)
        {
            printf("Error when opening file. Aborting.\n");
            continue;
        }

        fseeko64(dump, *p, SEEK_SET);
        file_pos_t total_written = 0;
        while(!feof(dump) && total_written < sig->sz)
        {
            size_t sz = fread(buffer, 1, BSIZE, dump);
            size_t szw = fwrite(buffer, 1, sz, dest);
            if(sz != szw)
            {
                printf("Error when writing file. Aborting.\n");
                break;
            }
            total_written += sz;
        }
        printf("%u bytes written.\n", (unsigned int)total_written);

        fclose(dest);
    }
    free(buffer);
}
