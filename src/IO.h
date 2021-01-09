/**
 * \file This file declares functions and types for io operations
 */
#pragma once

#if __linux__
#define _POSIX_C_SOURCE 200809L
#endif
#include <stdio.h>

typedef enum {
    ATT_IO_GET_FILE_SIZE_OK,
    ATT_IO_GET_FILE_SIZE_BAD_FP,
    ATT_IO_GET_FILE_SIZE_NOT_REGULAR_FILE,
} AttIOGetFileSizeError;

typedef struct {
    size_t file_size;
    AttIOGetFileSizeError error;
} AttIOGetFileSizeResult;

/**
 * \brief This function queries the size of an open regular file
 */
AttIOGetFileSizeResult AttIOGetFileSizeFP(FILE* file);

typedef enum {
    ATT_IO_READ_FILE_OK,
    ATT_IO_READ_FILE_OPEN_FAILED,
    ATT_IO_READ_FILE_READ_FAILED,
    ATT_IO_READ_FILE_OUT_OF_MEMORY,
    ATT_IO_READ_FILE_NOT_REGULAR_FILE,
} AttIOReadFileError;

typedef struct {
    unsigned char* buffer;
    size_t buffer_size;
    AttIOReadFileError error;
} AttIOReadFileBufferResult;

/**
 * \brief This function opens a regular file and reads its contents as a byte array
 */
AttIOReadFileBufferResult AttIOReadFileBuffer(char const* file_name);
