/**
 * \file This file declares functions and types for io operations
 */
#pragma once

#include "Error.h"
#include "Memory.h"

#if __linux__
#define _GNU_SOURCE
#endif

#include <stdio.h>

typedef enum {
    ATT_IO_FILE_TYPE_DISK,
    ATT_IO_FILE_TYPE_STREAM,
    ATT_IO_FILE_TYPE_UNKNOWN,
} AttIOFileType;

AttError AttIOGetFileType(FILE* restrict file, AttIOFileType* restrict result);
AttError AttIOGetDiskSize(FILE* restrict disk_file, size_t* restrict result);

/**
 * \brief Open a file and read its contents as a byte array
 */
AttError AttIOReadBuffer(char const* restrict file_name, AttDynamicBuffer* restrict result);
AttError AttIOReadDiskBuffer(FILE* restrict disk_file, AttBuffer* restrict result);
AttError AttIOReadFileBuffer(FILE* restrict file, AttDynamicBuffer* restrict result);
