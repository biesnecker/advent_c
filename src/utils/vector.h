#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "../macros.h"
#include "rand.h"

typedef struct vector {
    size_t capacity;
    size_t len;
    size_t elemSize;
    void* data;
} vector;

void vectorInit(vector* v, size_t capacity, size_t elemSize);

void vectorInitWithData(vector* v,
                        size_t capacity,
                        size_t elemSize,
                        const void* existingData,
                        size_t existingDataElems);

void vectorTruncate(vector* v);

void vectorFinalize(vector* v);

size_t vectorSize(const vector* v);

size_t vectorCapacity(const vector* v);

uint8_t* vectorDataPtr(const vector* v);

void vectorPush(vector* v, const void* elem);

void* vectorAt(const vector* v, size_t idx);

void* vectorBack(const vector* v);

void vectorPopBack(vector* v);

void vectorSort(vector* v, int (*cmp)(const void*, const void*));

void vectorInsertionSort(vector* v, int (*cmp)(const void*, const void*));

void vectorShuffle(vector* v, rng* r);

void* vectorBinarySearch(const vector* v,
                         const void* key,
                         int (*cmp)(const void*, const void*));

#define _VECTOR_IDX_VAR TOKEN_PASTE(__i_, __LINE__)
#define _VECTOR_COUNT_VAR TOKEN_PASTE(__count_, __LINE__)

#define VECTOR_FOREACH_TYPED(VECTOR, TYPE, VAR)                                \
    for (size_t _VECTOR_IDX_VAR = 0, _VECTOR_COUNT_VAR = (VECTOR)->len;        \
         _VECTOR_IDX_VAR == 0 && _VECTOR_COUNT_VAR > 0;)                       \
        for (TYPE * (VAR) = (TYPE*)vectorAt((VECTOR), 0);                      \
             ++_VECTOR_IDX_VAR,                                                \
                    (_VECTOR_IDX_VAR - 1) < _VECTOR_COUNT_VAR &&               \
                        ((VAR) = vectorAt((VECTOR), (_VECTOR_IDX_VAR - 1)),    \
                        true);)

#define VECTOR_FOREACH(VECTOR, VAR) VECTOR_FOREACH_TYPED(VECTOR, void, VAR)

// These macros are included so that data structures that use vectors as their
// underlying store can get all the benefits of inlining despite the vactor code
// being in a different translation unit.

#define VECTOR_SIZE(v) ((v)->len)
#define VECTOR_CAPACITY(v) ((v)->capacity)
#define VECTOR_ELEM_SIZE(v) ((v)->elemSize)
#define VECTOR_DATA_PTR(v) ((uint8_t*)((v)->data))
#define VECTOR_AT(v, idx) (VECTOR_DATA_PTR(v) + (VECTOR_ELEM_SIZE(v) * (idx)))
#define VECTOR_BACK(v) VECTOR_AT(v, VECTOR_SIZE(v) - 1)
