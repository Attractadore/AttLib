#include "IO.h"

#include <assert.h>
#include <stdlib.h>

#if __linux__
#include <errno.h>

#include <sys/stat.h>
#endif

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

#ifdef __linux__
    {
        const int fd = fileno(file);
        assert(fd != -1);
        struct stat file_info = {0};
        const int fstat_res = fstat(fd, &file_info);
        if (fstat_res == -1) {
            assert(errno == ENOMEM);
            res.error = ATT_IO_READ_FILE_OUT_OF_MEMORY;
            goto cleanup;
        }
        if (!S_ISREG(file_info.st_mode)) {
            res.error = ATT_IO_READ_FILE_NOT_REGULAR_FILE;
            goto cleanup;
        }
        res.buffer_size = file_info.st_size;
    }
#else
    assert(!"This function is currently only available on Linux");
#endif

    res.buffer = calloc(res.buffer_size, sizeof(*res.buffer));
    if (!res.buffer) {
        res.error = ATT_IO_READ_FILE_OUT_OF_MEMORY;
        goto cleanup;
    }

    {
        const size_t buffer_length = fread(res.buffer, sizeof(*res.buffer), res.buffer_size, file);
        if (buffer_length != res.buffer_size) {
            res.error = ATT_IO_READ_FILE_READ_FAILED;
            goto cleanup;
        }
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
