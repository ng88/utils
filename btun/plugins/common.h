
#ifndef PLUGIN_COMMON_H
#define PLUGIN_COMMON_H

#include <stdlib.h>

#include "../plugin_def.h"
#include "../assert.h"

#define MIN_BUF_SIZE 256

size_t next_power_of_two(size_t k);
void ensure_buffer_size(plugin_info_t * p, size_t s);

#endif
