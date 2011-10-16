/***************************************************************************
 * Tar Stream Format                                                       *
 * Tar-like archiving that can be easily streamed over HTTP                *
 ***************************************************************************/
 
/***************************************************************************
 *   This file is part of the 'utils' projects                             *
 *                                                                         *
 *   'utils' projects                                                      *
 *                                                                         *
 *   Copyright (C) 2006, 2008 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://svn.ngsoft-fr.com/trac/utils/                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/                                                                

#define _GNU_SOURCE

#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <zlib.h>

#include "tsf.h"
#include "assert.h"


enum
{
    TSF_ST_BSIZE = 65536,
    TSF_ST_ZBSIZE = (size_t)(((float)TSF_ST_BSIZE) * 1.01 + 12.00) + 1
};


struct _tsf_archive_s
{
    char * name;
    int fd;
    tsf_file_header_t  h;
    tsf_options_t options;
    
    char buffer_in[TSF_ST_BSIZE];
    char buffer_out[TSF_ST_ZBSIZE];
};

void tsf_init_file_header(tsf_file_header_t * dest);

tsf_archive_t * tsf_open_archive(const char * name, tsf_mode_t m, tsf_options_t * options)
{
    if(m == TSF_READ)
    {
	printf("read not yet implemented\n");
	return 0;
    }

    int fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(fd < 0)
	return 0;

    tsf_archive_t * a = (tsf_archive_t*)malloc(sizeof(tsf_archive_t));
    c_assert(a);
    memset((char*)a, 0, sizeof(tsf_archive_t));

    a->name = strdup(name);
    a->fd = fd;

    if(options)
	a->options = *options;
    else
    {
	a->options.cpr_level = 5;
	a->options.tree_flags = 0;
	a->options.verbose_level = 0;
    }

    return a;
}

void tsf_close_archive(tsf_archive_t * a)
{
    if(!a)
	return;

    close(a->fd);
    free(a->name);
    free(a);
}

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

static const char * ensure_relative_path(const char * p)
{
    if(p[0] == '/')
	return p + 1; // skip unix root
    else if(isalpha(p[0]) && p[1] == ':' && (p[2] == '/' || p[2] == '\\'))
	return p + 3; // skip win root
    else
	return p;
}



void tsf_init_file_header(tsf_file_header_t * dest)
{
    c_assert(dest);
    dest->magic = &TSF_MAGIC[0];
    dest->version = TSF_VERSION;
    dest->compat_version = TSF_VERSION;
    dest->entry_count = 0;
    dest->extract_size = 0;
    dest->archive_size = TSF_FILE_HEADER_SIZE;
}

void tsf_begin_entries(tsf_archive_t * a)
{
    c_assert(a);
    tsf_init_file_header(&a->h);
    lseek(a->fd, TSF_FILE_HEADER_SIZE, SEEK_SET);
}

void tsf_end_entries(tsf_archive_t * a)
{
    c_assert(a);

    lseek(a->fd, 0, SEEK_SET);

    write(a->fd, a->h.magic, TSF_MAGIC_SIZE);
    write16(a->fd, a->h.version);
    write16(a->fd, a->h.compat_version);
    write32(a->fd, a->h.entry_count);
    write32(a->fd, a->h.extract_size);
    write32(a->fd, a->h.archive_size);

    if(a->options.verbose_level > 1)
	printf("Archive size ratio = %d / %d\n", a->h.archive_size, a->h.extract_size);
    if(a->options.verbose_level > 0)
	printf("Archive size ratio = %.2g\n", (float)a->h.archive_size / (float)a->h.extract_size);

}

int tsf_append_file_entry(tsf_archive_t * a, const char * file)
{
    c_assert(a && file);

    if(strcmp(file, a->name) == 0)
    {
	// trying to add archive to itself
	if(a->options.verbose_level > 0)
	    printf("Skipping myself (%s)\n", file);
	return 0;
    }

    int fdsrc = open(file, O_RDONLY);
    if(fdsrc < 0)
	return -1;

    file = ensure_relative_path(file);
    int n = strlen(file);
    uLong sum = 0;

    // skip header
    lseek(a->fd, TSF_ENTRY_HEADER_SIZE + n, SEEK_CUR);

    ssize_t totUncompressed = 0;
    ssize_t totCompressed = 0;
    ssize_t r;
    uint32_t format;
    if(a->options.cpr_level == 0)
    {
	format = TSF_EF_RAW;
        sum = adler32(0L, Z_NULL, 0);
	do
	{
	    r = read(fdsrc, a->buffer_in, TSF_ST_BSIZE);
	    sum = adler32(sum, (const Bytef *)a->buffer_in, r);
	    totUncompressed += r;
	    if(write(a->fd, a->buffer_in, r) != r)
	    {
		close(fdsrc);
		return -1;
	    }
	}
	while(r > 0);
	totCompressed = totUncompressed;
    }
    else
    {
	z_stream cs;
	format = TSF_EF_GZ;
	cs.zalloc = Z_NULL;
	cs.zfree = Z_NULL;
	cs.opaque = Z_NULL;
	if(deflateInit(&cs, a->options.cpr_level) != Z_OK)
	    return -1;

	do
	{
	    r = read(fdsrc, a->buffer_in, TSF_ST_BSIZE);

	    if(r > 0)
	    {
		cs.next_in = (Bytef *)&a->buffer_in[0];
		cs.avail_in = r;
		cs.next_out = (Bytef *)&a->buffer_out[0];
		cs.avail_out = TSF_ST_ZBSIZE;

		int status = deflate(&cs, Z_SYNC_FLUSH);
		int out_sz = TSF_ST_ZBSIZE - cs.avail_out;

		if(status != Z_OK && status != Z_STREAM_END)
		    return -1;
		
		if(write(a->fd, a->buffer_out, out_sz) != out_sz)
		{
		    close(fdsrc);
		    return -1;
		}
	    }
	}
	while(r > 0);
	deflateEnd(&cs);
	sum = cs.adler;
	totCompressed = cs.total_out;
	totUncompressed = cs.total_in;
    }

    close(fdsrc);

   if(a->options.verbose_level > 1)
       printf(" + file %s [format=%d, sum=%X, level=%d, ratio=%.2g]\n", file,
	      format, (unsigned)sum, a->options.cpr_level, (float)totCompressed / (float)totUncompressed);

    // go back to header
    lseek(a->fd, -(totCompressed + TSF_ENTRY_HEADER_SIZE + n), SEEK_CUR);

    write32(a->fd, totCompressed);
    write32(a->fd, totUncompressed);
    write32(a->fd, format);
    write32(a->fd, 0);
    write32(a->fd, sum);
    write16(a->fd, n);
    write(a->fd, file, n);

    // go to data end
    lseek(a->fd, totCompressed, SEEK_CUR);

    a->h.entry_count++;
    a->h.extract_size += totUncompressed;
    a->h.archive_size += TSF_ENTRY_HEADER_SIZE + n + totCompressed;

    return 0;
}


int tsf_append_folder_entry(tsf_archive_t * a, const char * folder)
{
    c_assert(a && folder);

    folder = ensure_relative_path(folder);

    if(a->options.verbose_level > 1)
	printf(" + folder %s\n", folder);

    int n = strlen(folder);

    write32(a->fd, 0);
    write32(a->fd, 0);
    write32(a->fd, TSF_EF_RAW);
    write32(a->fd, 1);
    write32(a->fd, 0); // no CRC for folders
    write16(a->fd, n);
    write(a->fd, folder, n);

    a->h.entry_count++;
    a->h.archive_size += TSF_ENTRY_HEADER_SIZE + n;

    return 0;

}


static tsf_archive_t * off_a;
static int on_file_found(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
    int r = 0;
    (void)ftwbuf;
    (void)sb;
    switch(tflag)
    {
    case FTW_F:
	r = tsf_append_file_entry(off_a, fpath);
	break;
    case FTW_D:
	if(off_a->options.skip_svn && strcmp(".svn", fpath + ftwbuf->base) == 0)
	{
	    if(off_a->options.verbose_level > 0)
		printf("Skipping %s\n", fpath);

	    return FTW_SKIP_SUBTREE;
	}

	r = tsf_append_folder_entry(off_a, fpath);
	break;
    }
    if(r < 0)
	return FTW_STOP;
    else
	return FTW_CONTINUE;
}

int tsf_append_tree_entries(tsf_archive_t * a, const char * dir)
{
    c_assert(a);

    off_a = a;

    if (nftw(dir, &on_file_found, 20, a->options.tree_flags | FTW_ACTIONRETVAL) == FTW_STOP)
	return -1;

    return 0;
}

