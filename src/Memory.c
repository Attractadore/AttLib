#include "Memory.h"

#include <stdlib.h>

const size_t ATT_BUFFER_DEFAULT_CAPACITY = 1024;
const double ATT_BUFFER_DEFAULT_GROW_FACTOR = 1.5;

void* AttAlloc(const size_t num, const size_t size) {
#if __linux__
    return calloc(num, size);
#else
    return malloc(num * size);
#endif
}

void* AttByteAlloc(const size_t num_bytes) {
    return AttAlloc(num_bytes, sizeof(unsigned char));
}

void* AttCalloc(const size_t num, const size_t size) {
    return calloc(num, size);
}

void* AttByteCalloc(const size_t num_bytes) {
    return AttCalloc(num_bytes, sizeof(unsigned char));
}

void* AttRealloc(void* const data, const size_t new_num, const size_t size) {
    return realloc(data, new_num * size);
}

void* AttByteRealloc(void* const data, const size_t new_num_bytes) {
    return AttRealloc(data, new_num_bytes, sizeof(unsigned char));
}

void AttFree(void* p) {
    free(p);
}

bool AttBufferAlloc(AttBuffer* const buffer, const size_t capacity) {
    buffer->capacity = capacity;
    buffer->data = AttByteAlloc(capacity);
    return buffer->data;
}

bool AttDynamicBufferAllocDefault(AttDynamicBuffer* const buffer) {
    buffer->size = 0;
    return AttBufferAlloc(&buffer->buffer, ATT_BUFFER_DEFAULT_CAPACITY);
}

bool AttBufferRealloc(AttBuffer* const buffer, const size_t new_capacity) {
    unsigned char* new_data = AttByteRealloc(buffer->data, new_capacity);
    if (new_data) {
        buffer->data = new_data;
        buffer->capacity = new_capacity;
    }
    return new_data;
}

bool AttBufferGrow(AttBuffer* const buffer) {
    return AttBufferRealloc(buffer, buffer->capacity * ATT_BUFFER_DEFAULT_GROW_FACTOR);
}

void AttBufferFree(AttBuffer* const buffer) {
    AttFree(buffer->data);
    buffer->data = NULL;
    buffer->capacity = 0;
}

void AttDynamicBufferFree(AttDynamicBuffer* const buffer) {
    AttBufferFree(&buffer->buffer);
    buffer->size = 0;
}
