#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    unsigned char* data;
    size_t capacity;
} AttBuffer;

typedef struct {
    union {
        AttBuffer buffer;
        struct {
            unsigned char* data;
            size_t capacity;
        };
    };
    size_t size;
} AttDynamicBuffer;

extern const size_t ATT_BUFFER_DEFAULT_CAPACITY;
extern const double ATT_BUFFER_DEFAULT_GROW_FACTOR;

void* AttAlloc(size_t num, size_t size);
void* AttByteAlloc(size_t num_bytes);
void* AttCalloc(size_t num, size_t size);
void* AttByteCalloc(size_t num_bytes);
void* AttRealloc(void* data, size_t new_num, size_t size);
void* AttByteRealloc(void* data, size_t new_num_bytes);
void AttFree(void* p);

bool AttBufferAlloc(AttBuffer* buffer, size_t capacity);
bool AttDynamicBufferAllocDefault(AttDynamicBuffer* buffer);
bool AttBufferRealloc(AttBuffer* buffer, size_t new_capacity);
bool AttBufferGrow(AttBuffer* buffer);
void AttBufferFree(AttBuffer* buffer);
void AttDynamicBufferFree(AttDynamicBuffer* buffer);
