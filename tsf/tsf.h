#ifndef TSF_H
#define TSF_H

/**
 * Tar Stream Format
 * Tar-like archiving that can be easily streamed ofdsver HTTP
 *
 * Author: Nicolas GUILLAUME
 *
 */

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
    uint32_t checksum; // CRC 32 of data (raw) or adler 32 (gz) 
    uint16_t name_size;
    const char * name; // strlen(name) == name_size
} tsf_entry_header_t;

struct stat;

void tsf_init_file_header(tsf_file_header_t * dest);

void tsf_begin_entries(int fddest, tsf_file_header_t * h);
/**
 * level: compress level, 0 = uncompressed
 * return -1 on error
 */
int tsf_append_file_entry(int fddest, tsf_file_header_t * h, const char * file, int level);
// return -1 on error
int tsf_append_folder_entry(int fddest, tsf_file_header_t * h, const char * folder);
// return -1 on error, not reetrant
int tsf_append_tree_entries(int fddest, tsf_file_header_t * h, const char * dir, int flags);
void tsf_end_entries(int fddest, tsf_file_header_t * h);

#endif
