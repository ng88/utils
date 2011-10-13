
#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

#include <arpa/inet.h>


#include "tsf.h"
#include "assert.h"


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
    dest->entry_count = 0;
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
    write32(fddest, h->entry_count);
    write32(fddest, h->extract_size);
    write32(fddest, h->archive_size);
    printf("%d %d\n",h->extract_size,h->archive_size);  
}

int tsf_append_file_entry(int fddest, tsf_file_header_t * h, const char * file, const struct stat * st)
{
    c_assert(h && file && st);

    int fdsrc = open(file, O_RDONLY);
    if(fdsrc < 0)
	return -1;

    enum { BSIZE = 1024 };
    char buffer[BSIZE];

    int n = strlen(file);

    write32(fddest, st->st_size);
    write32(fddest, st->st_size);
    write32(fddest, TSF_EF_RAW);
    write32(fddest, 0);
    write32(fddest, 0);
    write16(fddest, n);
    write(fddest, file, n);

    h->entry_count++;
    h->extract_size += st->st_size;
    h->archive_size += TSF_ENTRY_HEADER_SIZE + n + st->st_size;

    ssize_t tot = 0;
    ssize_t r;
    do
    {
	r = read(fdsrc, buffer, BSIZE);
	tot += r;
	if(write(fddest, buffer, r) != r)
	{
	    close(fdsrc);
	    return -1;
	}
    }
    while(r > 0);

    close(fdsrc);

    return tot == st->st_size ? 0 : -1;
}

int tsf_append_folder_entry(int fddest, tsf_file_header_t * h, const char * folder)
{
    c_assert(h && folder);

    int n = strlen(folder);

    write32(fddest, 0);
    write32(fddest, 0);
    write32(fddest, TSF_EF_RAW);
    write32(fddest, 1);
    write32(fddest, 0);
    write16(fddest, n);
    write(fddest, folder, n);

    h->entry_count++;
    h->archive_size += TSF_ENTRY_HEADER_SIZE + n;

    return 0;

}


static int off_fd;
static tsf_file_header_t * off_h;
static int on_file_found(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
    int r = 0;
    switch(tflag)
    {
    case FTW_F:
	r = tsf_append_file_entry(off_fd, off_h, fpath, sb);
	break;
    case FTW_D:
	r = tsf_append_folder_entry(off_fd, off_h, fpath);
	break;
    }
    return r;
}

int tsf_append_tree_entries(int fddest, tsf_file_header_t * h, const char * dir, int flags)
{
    off_fd = fddest;
    off_h = h;

    if (nftw(dir, &on_file_found, 20, flags) == -1)
	return -1;

    return 0;
}

