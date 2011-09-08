#include "sigs.h"


/** Signature list */
const signature_t SIGS[] =
{
  {"\xFF\xD8\xFF\xE0", 4, 0, "jpg", 8 * MiB},
  {"\xFF\xD8\xFF\xE1", 4, 0, "jpg", 8 * MiB},
  {"ftypqt", 6, 4, "mov", 64 * MiB},
  {"ftypM4V", 7, 4, "m4v", 64 * MiB},
  {NULL, 0, 0, NULL, 0},
};

int sig_is_null(const signature_t * s)
{
    return s == NULL || s->sig == NULL;
}
