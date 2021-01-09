#include "IO.h"

#include <assert.h>
#include <stdlib.h>

#if __linux__
#include <sys/stat.h>
#endif

AttIOGetFileSizeResult AttIOGetFileSizeFP(FILE* const file) {
    assert(file);

    AttIOGetFileSizeResult res = {
        .file_size = 0,
        .error = ATT_IO_GET_FILE_SIZE_OK,
    };

#if __linux__

    const int fd = fileno(file);
    if (fd == -1) {
        res.error = ATT_IO_GET_FILE_SIZE_BAD_FP;
        return res;
    }

    struct stat file_info = {0};
    fstat(fd, &file_info);
    if (!S_ISREG(file_info.st_mode)) {
        res.error = ATT_IO_GET_FILE_SIZE_NOT_REGULAR_FILE;
        return res;
    }

    res.file_size = file_info.st_size;

#else

    assert(!"File size from FP available only on POSIX systems");

#endif

    return res;
}

AttIOReadFileBufferResult AttIOReadFileBuffer(char const* const file_name) {
    assert(file_name);

    AttIOReadFileBufferResult res = {
        .buffer = NULL,
        .buffer_size = 0,
        ATT_IO_READ_FILE_OK,
    };

    FILE* const file = fopen(file_name, "rb");
    if (!file) {
        res.error = ATT_IO_READ_FILE_OPEN_FAILED;
        goto cleanup;
    }

    const AttIOGetFileSizeResult get_file_size_res = AttIOGetFileSizeFP(file);
    assert(get_file_size_res.error != ATT_IO_GET_FILE_SIZE_BAD_FP);
    if (get_file_size_res.error == ATT_IO_GET_FILE_SIZE_NOT_REGULAR_FILE) {
        res.error = ATT_IO_READ_FILE_NOT_REGULAR_FILE;
        goto cleanup;
    }
    res.buffer_size = get_file_size_res.file_size;

    res.buffer = calloc(res.buffer_size, sizeof(*res.buffer));
    if (!res.buffer) {
        res.error = ATT_IO_READ_FILE_OUT_OF_MEMORY;
        goto cleanup;
    }

    const size_t buffer_length = fread(res.buffer, sizeof(*res.buffer), res.buffer_size, file);
    if (buffer_length != res.buffer_size) {
        res.error = ATT_IO_READ_FILE_READ_FAILED;
        goto cleanup;
    }

    fclose(file);

    return res;

cleanup:
    if (file) {
        fclose(file);
    }
    free(res.buffer);
    res.buffer_size = 0;

    return res;
}
