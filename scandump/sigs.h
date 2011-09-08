#ifndef SIGS_H
#define SIGS_H

#include <stdlib.h>
#include <unistd.h>


/** @author Nicolas GUILLAUME
 */

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


enum
{
  KiB = 1024,
  MiB = 1024 * KiB,
  GiB = 1024 * MiB,
};


/** Signature list, NULL terminated */
extern const signature_t SIGS[];


int sig_is_null(const signature_t * s);

#endif
