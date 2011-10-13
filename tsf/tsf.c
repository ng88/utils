
#include "tsf.h"

/**
 * Tar Stream Format
 * Tar-like archiving that can be easily streamed over HTTP
 *
 * Author: Nicolas GUILLAUME
 *
 */

static inline void write32(int fd, uint32_t v)
{
    v = htonl(v);
    write(fd, &v, sizeof(v));
}

static inline void write16(int fd, uint16_t v)
{
    v = htons(v);
    write(fd, (void*)&v, sizeof(v));
}

void tsf_init_file_header(tsf_file_header_t * dest)
{
    c_assert(dest);
    dest->magic = &TSF_MAGIC[0];
    dest->version = TSF_VERSION;
    dest->file_count = 0;
    dest->extract_size = 0;
    dest->archive_size = TSF_FILE_HEADER_SIZE;
}

void tsf_begin_entries(int fddest, tsf_file_header_t * h)
{
    c_assert(h);
    tsf_init_file_header(h);
    lseek(fddest, TSF_FILE_HEADER_SIZE, SEEK_SET);
}

void tsf_end_entries(int fddest, tsf_file_header_t * h)
{
    c_assert(h);

    lseek(fddest, 0, SEEK_SET);

    write(fddest, h->magic, TSF_MAGIC_SIZE);
    write16(fddest, h->version);
    write32(fddest, h->file_count);
    write32(fddest, h->extract_size);
    write32(fddest, h->archive_size);

}

void tsf_append_file_entry(int fddest, tsf_file_header_t * h, const char * file, struct stat * st)
{
    c_assert(h && file);

    write32(fddest, h->archive_size);
}


void tsf_append_rec_dir()
{
}
