#include "hashset.h"

#include <assert.h>
#include <string.h>

typedef struct bucket {
    uint64_t hash;
} bucket;

static inline uint8_t* _dataBytePtr(const hashset* h) {
    return (uint8_t*)h->data;
}

static inline bucket* _bucketForIndex(const hashset* h, size_t idx) {
    return (bucket*)(_dataBytePtr(h) + (h->stride * idx));
}

static inline void* _elementForBucket(const bucket* b) {
    return ((uint8_t*)b) + sizeof(bucket);
}

static inline size_t _nextIndex(const hashset* h, size_t idx) {
    return (idx + 1) % h->capacity;
}

void hashsetInit(hashset* h,
                 size_t capacity,
                 size_t elemSize,
                 hashsetComp comp,
                 hashsetHash hash,
                 hashsetFree free) {
    // Must be power of two.
    assert((capacity & (capacity - 1)) == 0);
    h->len = 0;
    h->capacity = capacity;
    h->elemSize = elemSize;
    h->stride = elemSize + sizeof(bucket);
    h->comp = comp;
    h->hash = hash;
    h->free = free;
    h->data = calloc(capacity, h->stride);
    assert(h->data != NULL);
}

void hashsetFinalize(hashset* h) {
    if (h->free != NULL) {
        for (size_t i = 0; i < h->capacity; ++i) {
            bucket* b = _bucketForIndex(h, i);
            if (b->hash != 0) {
                h->free(_elementForBucket(b));
            }
        }
    }
    free(h->data);
}

size_t hashsetSize(const hashset* h) {
    return h->len;
}

size_t hashsetCapacity(const hashset* h) {
    return h->capacity;
}

static bucket*
_findBucketWithHash(hashset* h, uint64_t hash, const void* elem) {
    size_t idx = hash & (h->capacity - 1);
    while (true) {
        bucket* b = _bucketForIndex(h, idx);
        if (b->hash == 0 ||
            (b->hash == hash && h->comp(elem, _elementForBucket(b)) == 0)) {
            return b;
        }
        idx = _nextIndex(h, idx);
    }
}

static bucket* _findBucket(hashset* h, const void* elem) {
    return _findBucketWithHash(h, h->hash(elem), elem);
}

bool hashsetContains(hashset* h, const void* elem) {
    bucket* b = _findBucket(h, elem);
    return b->hash != 0;
}

void* hashsetLookup(hashset* h, const void* elem) {
    bucket* b = _findBucket(h, elem);
    return b->hash == 0 ? NULL : _elementForBucket(b);
}

static void*
_insertImpl(hashset* h, bucket* b, uint64_t hash, const void* elem) {
    void* bucketElement = _elementForBucket(b);
    if (b->hash != 0 && h->free != NULL) {
        h->free(bucketElement);
    }
    b->hash = hash;
    memcpy(bucketElement, elem, h->elemSize);
    h->len += 1;
    return bucketElement;
}

static void _grow(hashset* h, size_t newCapacity) {
    hashset h2;
    hashsetInit(&h2, newCapacity, h->elemSize, h->comp, h->hash, h->free);
    for (size_t i = 0; i < h->capacity; ++i) {
        bucket* originalBucket = _bucketForIndex(h, i);
        if (originalBucket->hash != 0) {
            void* elem = _elementForBucket(originalBucket);
            bucket* newBucket =
                _findBucketWithHash(&h2, originalBucket->hash, elem);
            _insertImpl(&h2, newBucket, originalBucket->hash, elem);
        }
    }
    free(h->data);
    h->data = h2.data;
    h->capacity = newCapacity;
}

void* hashsetInsert(hashset* h, const void* elem) {
    if (h->len >= (h->capacity / 4) * 3) {
        _grow(h, h->capacity << 1);
    }
    uint64_t hash = h->hash(elem);
    bucket* b = _findBucketWithHash(h, hash, elem);
    return _insertImpl(h, b, hash, elem);
}

void* hashsetLookupOrInsert(hashset* h, const void* elem, bool* didInsert) {
    uint64_t hash = h->hash(elem);
    bucket* b = _findBucketWithHash(h, hash, elem);
    if (b->hash == 0) {
        *didInsert = true;
        if (h->len >= (h->capacity / 4) * 3) {
            _grow(h, h->capacity << 1);
        }
        return _insertImpl(h, b, hash, elem);
    } else {
        *didInsert = false;
        return _elementForBucket(b);
    }
}
