#include "vector.h"

#include <assert.h>
#include <string.h>

#include "rand.h"

void vectorInit(vector* v,
                size_t capacity,
                size_t elemSize,
                void (*free)(void*)) {
    assert(capacity > 0 && elemSize > 0);
    v->capacity = capacity;
    v->elemSize = elemSize;
    v->len = 0;
    v->data = malloc(capacity * elemSize);
    assert(v->data != NULL);
    v->free = free;
}

void vectorInitWithData(vector* v,
                        size_t capacity,
                        size_t elemSize,
                        const void* existingData,
                        size_t existingDataElems,
                        void (*free)(void*)) {
    assert(capacity > 0 && elemSize > 0 && existingDataElems > 0 &&
           capacity >= existingDataElems);
    v->capacity = capacity;
    v->elemSize = elemSize;
    v->len = existingDataElems;
    v->data = malloc(capacity * elemSize);
    assert(v->data != NULL);
    memcpy(v->data, existingData, elemSize * existingDataElems);
    v->free = free;
}

static void _freeAllElements(vector* v) {
    if (v->free != NULL) {
        VECTOR_FOREACH(v, elem) {
            v->free(elem);
        }
    }
}

void vectorTruncate(vector* v) {
    _freeAllElements(v);
    v->len = 0;
}

void vectorFinalize(vector* v) {
    _freeAllElements(v);
    v->capacity = 0;
    v->elemSize = 0;
    v->len = 0;
    free(v->data);
    v->data = NULL;
}

size_t vectorSize(const vector* v) {
    return v->len;
}

size_t vectorCapacity(const vector* v) {
    return v->capacity;
}

uint8_t* vectorDataPtr(const vector* v) {
    return (uint8_t*)v->data;
}

static void _vectorGrow(vector* v) {
    size_t newCapacity = v->capacity * 2;
    void* newData = realloc(v->data, newCapacity * v->elemSize);
    assert(newData != NULL);
    v->capacity = newCapacity;
    v->data = newData;
}

void vectorPush(vector* v, const void* elem) {
    if (v->len == v->capacity) {
        _vectorGrow(v);
    }
    memcpy(vectorDataPtr(v) + (v->len * v->elemSize), elem, v->elemSize);
    ++v->len;
}

void* vectorAt(const vector* v, size_t idx) {
    assert(idx >= 0 && idx < v->len);
    return vectorDataPtr(v) + (idx * v->elemSize);
}

void* vectorBack(const vector* v) {
    return vectorAt(v, v->len - 1);
}

void vectorPopBack(vector* v) {
    assert(v->len > 0);
    if (v->free != NULL) {
        v->free(VECTOR_AT(v, v->len - 1));
    }
    --v->len;
}

void vectorSort(vector* v, int (*cmp)(const void*, const void*)) {
    qsort(v->data, v->len, v->elemSize, cmp);
}

void vectorInsertionSort(vector* v, int (*cmp)(const void*, const void*)) {
    if (v->len < 2) {
        return;
    }

    uint8_t b[v->elemSize];
    void* buffer = &b;

    for (size_t i = 1; i < v->len; ++i) {
        size_t j = i;
        while (j > 0 && cmp(VECTOR_AT(v, j), VECTOR_AT(v, j - 1)) < 0) {
            memcpy(buffer, VECTOR_AT(v, j), v->elemSize);
            memcpy(VECTOR_AT(v, j), VECTOR_AT(v, j - 1), v->elemSize);
            memcpy(VECTOR_AT(v, j - 1), buffer, v->elemSize);
            --j;
        }
    }
}

void vectorShuffle(vector* v, rng* r) {
    if (v->len < 2) {
        return;
    }
    uint8_t* buffer = malloc(v->elemSize);
    uint8_t* bytes = vectorDataPtr(v);
    for (int i = v->len - 1; i > 0; --i) {
        uint32_t j = rngRandomBounded(r, i);
        uint8_t* iptr = bytes + (i * v->elemSize);
        uint8_t* jptr = bytes + (j * v->elemSize);
        memcpy(buffer, iptr, v->elemSize);
        memcpy(iptr, jptr, v->elemSize);
        memcpy(jptr, buffer, v->elemSize);
    }
    free(buffer);
}

void* vectorBinarySearch(const vector* v,
                         const void* key,
                         int (*cmp)(const void*, const void*)) {
    if (v->len == 0) {
        return NULL;
    }
    return bsearch(key, v->data, v->len, v->elemSize, cmp);
}
