#include "IO.h"

#include <assert.h>
#include <stdlib.h>

#if __linux__
#include <sys/stat.h>
#endif

AttError AttIOGetFileType(FILE* const file, AttIOFileType* const result) {
    assert(file);
    assert(result);

#ifdef __linux__
    const int fd = fileno(file);
    assert(fd != -1);
    struct stat file_info = {0};
    if (fstat(fd, &file_info)) {
        return ATT_ERROR_OS_ERROR;
    }
    switch (file_info.st_mode & S_IFMT) {
        case S_IFREG:
            *result = ATT_IO_FILE_TYPE_DISK;
            break;
        case S_IFCHR:
        case S_IFIFO:
        case S_IFSOCK:
            *result = ATT_IO_FILE_TYPE_STREAM;
            break;
        default:
            *result = ATT_IO_FILE_TYPE_UNKNOWN;
    }
#else
    assert(!"This function is currently only available on Linux");
#endif

    return ATT_OK;
}

AttError AttIOGetDiskSize(FILE* const disk_file, size_t* const result) {
    assert(disk_file);
    assert(result);
#ifndef NDEBUG
    {
        AttIOFileType file_type = ATT_IO_FILE_TYPE_UNKNOWN;
        if (AttIOGetFileType(disk_file, &file_type) == ATT_OK) {
            assert(file_type == ATT_IO_FILE_TYPE_DISK);
        }
    }
#endif

#ifdef __linux__
    const int fd = fileno(disk_file);
    assert(fd != -1);
    struct stat file_info = {0};
    if (fstat(fd, &file_info)) {
        return ATT_ERROR_OS_ERROR;
    }
    *result = file_info.st_size;
#else
    assert(!"This function is currently only available on Linux");
#endif

    return ATT_OK;
}

AttError AttIOReadBuffer(char const* const file_name, AttDynamicBuffer* const result) {
    assert(file_name);
    assert(result);

    AttError err = ATT_OK;

    FILE* const file = fopen(file_name, "rb");
    if (!file) {
        return ATT_ERROR_OPEN_FAILED;
    }

    AttIOFileType file_type = ATT_IO_FILE_TYPE_UNKNOWN;
    err = AttIOGetFileType(file, &file_type);
    if (err) {
        fclose(file);
        return err;
    }

    switch (file_type) {
        case ATT_IO_FILE_TYPE_DISK:
            err = AttIOReadDiskBuffer(file, &(result->buffer));
            result->size = result->capacity;
            break;
        case ATT_IO_FILE_TYPE_STREAM:
            err = AttIOReadFileBuffer(file, result);
            break;
        default:
            err = ATT_ERROR_INVALID_FILE_TYPE;
    }

    fclose(file);

    return err;
}

AttError AttIOReadDiskBuffer(FILE* const disk_file, AttBuffer* const result) {
    assert(disk_file);
    assert(result);
#ifndef NDEBUG
    {
        AttIOFileType file_type = ATT_IO_FILE_TYPE_UNKNOWN;
        if (AttIOGetFileType(disk_file, &file_type) == ATT_OK) {
            assert(file_type == ATT_IO_FILE_TYPE_DISK);
        }
    }
#endif

    AttError err = ATT_OK;

    size_t file_size = 0;
    err = AttIOGetDiskSize(disk_file, &file_size);
    if (err) {
        return err;
    }
    if (!AttBufferAlloc(result, file_size)) {
        return ATT_ERROR_OS_ERROR;
    }

    clearerr(disk_file);
    fread(result->data, sizeof(*result->data), result->capacity, disk_file);
    if (ferror(disk_file)) {
        AttBufferFree(result);
        return ATT_ERROR_READ_FAILED;
    }

    return ATT_OK;
}

AttError AttIOReadFileBuffer(FILE* const file, AttDynamicBuffer* const result) {
    assert(file);
    assert(result);
#ifndef NDEBUG
    {
        AttIOFileType file_type = ATT_IO_FILE_TYPE_UNKNOWN;
        if (AttIOGetFileType(file, &file_type) == ATT_OK) {
            assert(file_type == ATT_IO_FILE_TYPE_DISK ||
                   file_type == ATT_IO_FILE_TYPE_STREAM);
        }
    }
#endif

    if (!AttDynamicBufferAllocDefault(result)) {
        return ATT_ERROR_OS_ERROR;
    }

    clearerr(file);
    int c = EOF;
    while ((c = getchar()) != EOF) {
        if (result->size == result->capacity) {
            if (!AttBufferGrow(&result->buffer)) {
                AttBufferFree(&result->buffer);
                return ATT_ERROR_OS_ERROR;
            }
        }
        result->data[result->size++] = c;
    }
    if (ferror(file)) {
        AttBufferFree(&result->buffer);
        return ATT_ERROR_READ_FAILED;
    }

    return ATT_OK;
}
