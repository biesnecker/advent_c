#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct ringbuffer {
    size_t len;
    size_t elemSize;
    size_t capacity;
    size_t writeIdx;
    size_t readIdx;
    void (*freeFn)(void*);
    void* data;
    bool isFull;
} ringbuffer;

void ringbufferInit(ringbuffer* r,
                    size_t capacity,
                    size_t elemSize,
                    void (*freeFn)(void*));

void ringbufferFinalize(ringbuffer* r);

size_t ringbufferSize(const ringbuffer* r);

size_t ringbufferCapacity(const ringbuffer* r);

bool ringbufferFull(const ringbuffer* r);

bool ringbufferEmpty(const ringbuffer* r);

void* ringbufferPush(ringbuffer* r, const void* elem);

void* ringbufferAt(const ringbuffer* r, size_t idx);

void ringbufferPop(ringbuffer* r);
