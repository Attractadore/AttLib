#pragma once

typedef enum {
    ATT_OK,
    ATT_ERROR_OPEN_FAILED,
    ATT_ERROR_READ_FAILED,
    ATT_ERROR_INVALID_FILE_TYPE,
    ATT_ERROR_OS_ERROR,
} AttError;
