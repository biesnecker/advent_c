#include "ringbuffer.h"

#include <assert.h>
#include <string.h>

static inline size_t _nextIndex(const ringbuffer* r, size_t idx) {
    return (idx + 1) % r->capacity;
}

static inline uint8_t* _bytePtr(const ringbuffer* r) {
    return (uint8_t*)r->data;
}

static inline void* _elementAt(const ringbuffer* r, size_t idx) {
    return _bytePtr(r) + (r->elemSize * idx);
}

static inline void* _elementOffsetFromReadIdx(const ringbuffer* r, size_t idx) {
    idx = (r->readIdx + idx) % r->capacity;
    return _bytePtr(r) + (r->elemSize * idx);
}

void ringbufferInit(ringbuffer* r,
                    size_t capacity,
                    size_t elemSize,
                    void (*freeFn)(void*)) {
    r->len = 0;
    r->elemSize = elemSize;
    r->capacity = capacity;
    r->writeIdx = 0;
    r->readIdx = 0;
    r->data = calloc(capacity, elemSize);
    assert(r->data != NULL);
    r->freeFn = freeFn;
    r->isFull = false;
}

void ringbufferFinalize(ringbuffer* r) {
    if (r->freeFn != NULL) {
        for (size_t i = 0; i < r->len; ++i) {
            r->freeFn(_elementOffsetFromReadIdx(r, i));
        }
    }
    free(r->data);
}

size_t ringbufferSize(const ringbuffer* r) {
    return r->len;
}

size_t ringbufferCapacity(const ringbuffer* r) {
    return r->capacity;
}

bool ringbufferFull(const ringbuffer* r) {
    return r->isFull;
}

bool ringbufferEmpty(const ringbuffer* r) {
    return !r->isFull && (r->readIdx == r->writeIdx);
}

void* ringbufferPush(ringbuffer* r, const void* elem) {
    assert(!r->isFull);
    void* dest = _elementAt(r, r->writeIdx);
    memcpy(dest, elem, r->elemSize);
    r->writeIdx = _nextIndex(r, r->writeIdx);
    // If the new writeIdx is the same as the readIdx we're full.
    r->isFull = r->writeIdx == r->readIdx;
    r->len += 1;
    return dest;
}

void* ringbufferAt(const ringbuffer* r, size_t idx) {
    assert(!ringbufferEmpty(r));
    assert(idx < r->len);
    return _elementOffsetFromReadIdx(r, idx);
}

void ringbufferPop(ringbuffer* r) {
    assert(!ringbufferEmpty(r));
    if (r->freeFn != NULL) {
        r->freeFn(_elementOffsetFromReadIdx(r, 0));
    }
    r->readIdx = _nextIndex(r, r->readIdx);
    r->len -= 1;
    r->isFull = false;
}
