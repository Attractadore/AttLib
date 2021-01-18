#include "IO.c"

#include "unity.h"

void test_AttIOReadBufferFileDoesNotExist() {
    char const* const file_name = "file_that_does_not_exist.txt";

    AttDynamicBuffer buffer = {0};
    const AttError err = AttIOReadBuffer(file_name, &buffer);
    TEST_ASSERT_EQUAL(ATT_ERROR_OPEN_FAILED, err);
    TEST_ASSERT_NULL(buffer.data);
    TEST_ASSERT_EQUAL_size_t(0, buffer.capacity);
    TEST_ASSERT_EQUAL_size_t(0, buffer.size);
}

void test_AttIOReadBufferFileExists() {
    char const* const file_name = "file_that_exists.txt";
    const char file_contents[] = "This is a file that exists\n";

    AttDynamicBuffer buffer = {0};
    const AttError err = AttIOReadBuffer(file_name, &buffer);
    TEST_ASSERT_EQUAL(ATT_OK, err);
    TEST_ASSERT_NOT_NULL(buffer.data);
    TEST_ASSERT_EQUAL_size_t(sizeof(file_contents) - 1, buffer.capacity);
    TEST_ASSERT_EQUAL_size_t(sizeof(file_contents) - 1, buffer.size);
    TEST_ASSERT_EQUAL_MEMORY(file_contents, buffer.data, buffer.capacity);
    TEST_ASSERT_EQUAL(ATT_OK, err);

    AttDynamicBufferFree(&buffer);
}
