
#ifndef PLUGIN_COMMON_H
#define PLUGIN_COMMON_H

#include <stdlib.h>

#include "../plugin_def.h"
#include "../assert.h"

/** Minimum buffer size */
#define MIN_BUF_SIZE 512

/** Return the next power of two of k. */
size_t next_power_of_two(size_t k);

/** Ensure that p->puffer is allocated and large enough to hold s bytes of data. */
void ensure_buffer_size(plugin_info_t * p, size_t s);

#endif
