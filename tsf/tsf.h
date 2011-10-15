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

#ifndef TSF_H
#define TSF_H

#include <stdint.h>

enum
{
    TSF_MAGIC_SIZE = 4,
    TSF_VERSION = 1
};

static const char TSF_MAGIC[TSF_MAGIC_SIZE] = {'T', 'S', 'F', '!'};

static const uint16_t TSF_FILE_HEADER_SIZE = TSF_MAGIC_SIZE+2+4+4+4;
static const uint16_t TSF_ENTRY_HEADER_SIZE = 4+4+4+4+4+2;



typedef struct
{
    const char * magic; // == TSF_MAGIC
    uint16_t version; // version of the file
    uint16_t compat_version; // max version that is compatible with this file
    uint32_t entry_count;
    uint32_t extract_size; // space needed to extract all files in bytes
    uint32_t archive_size; // size of this archive file in bytes (including headers)
} tsf_file_header_t;

typedef enum
{
    TSF_EF_RAW = 0, // uncompressed RAW data, if arg > 0 this is a folder
    TSF_EF_XOR = 1, // raw data xor (char)arg1
    TSF_EF_GZ  = 2, // raw data gzipped
} tsf_entry_format_t;

typedef struct
{
    uint32_t entry_size; // size of the block in the archive in bytes (excluding header)
    uint32_t extract_size; // size of the extracted file in bytes (same as entry_size if uncompressed)
    uint32_t format; // see tsf_entry_format_t
    uint32_t arg; // depends of selected format
    uint32_t checksum; //  adler32
    uint16_t name_size;
    const char * name; // strlen(name) == name_size
} tsf_entry_header_t;

typedef struct
{
    int cpr_level;  // compression level
    int tree_flags; // flags for nftw
    int verbose_level; // verbosity level
} tsf_options_t;

typedef enum
{
    TSF_READ,
    TSF_WRITE,
} tsf_mode_t;

// opaque internal structure
struct _tsf_archive_s;
typedef struct _tsf_archive_s tsf_archive_t;


///////// READ WRITE FUNCTIONS ///////////

/**
 * Open archive for reading or writting
 * name & options are copied (can be stack variables)
 */
tsf_archive_t * tsf_open_archive(const char * name, tsf_mode_t mode, tsf_options_t * options);

/**
 * Close archive
 */
void tsf_close_archive(tsf_archive_t * a);


///////// WRITE ONLY FUNCTIONS ///////////

/**
 * Begin write
 */
void tsf_begin_entries(tsf_archive_t * a);

/**
 * End write
 */
void tsf_end_entries(tsf_archive_t * a);


/**
 * Add a file to a
 * return -1 on error
 */
int tsf_append_file_entry(tsf_archive_t * a, const char * file);

/** Add a folder to the archive
 * return -1 on error
 */
int tsf_append_folder_entry(tsf_archive_t * a, const char * folder);


/** Add a full tree to a (including all files and folder recursively)
 * return -1 on error, not reetrant
 */
int tsf_append_tree_entries(tsf_archive_t * a, const char * dir);


///////// READ ONLY FUNCTIONS ///////////

#endif
