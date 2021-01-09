#include "IO.c"

#include "unity.h"

void test_AttIOReadFileBufferFileDoesNotExist() {
    const AttIOReadFileBufferResult res = AttIOReadFileBuffer("file_that_does_not_exist.txt");
    TEST_ASSERT_NULL(res.buffer);
    TEST_ASSERT_EQUAL_size_t(0, res.buffer_size);
    TEST_ASSERT_EQUAL(ATT_IO_READ_FILE_OPEN_FAILED, res.error);
}

void test_AttIOReadFileBufferFileExists() {
    char const* const file_name = "file_that_exists.txt";

    const AttIOReadFileBufferResult res = AttIOReadFileBuffer(file_name);

    TEST_ASSERT_NOT_NULL(res.buffer);

    const char file_contents[] = "This is a file that exists\n";

    TEST_ASSERT_EQUAL_size_t(sizeof(file_contents) - 1, res.buffer_size);
    TEST_ASSERT_EQUAL_MEMORY(file_contents, res.buffer, res.buffer_size);
    TEST_ASSERT_EQUAL(ATT_IO_READ_FILE_OK, res.error);

    free(res.buffer);
}
