#pragma once

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

void vectorFinalize(vector* v);

uint8_t* vectorDataPtr(vector* v);

void vectorPush(vector* v, const void* elem);

void* vectorAt(vector* v, size_t idx);

void* vectorBack(vector* v);

void vectorSort(vector* v, int (*cmp)(const void*, const void*));

void vectorShuffle(vector* v, rng* r);

void* vectorBinarySearch(vector* v,
                         const void* key,
                         int (*cmp)(const void*, const void*));

#define _VECTOR_IDX_VAR TOKEN_PASTE(__i_, __LINE__)
#define _VECTOR_COUNT_VAR TOKEN_PASTE(__count_, __LINE__)

#define VECTOR_FOR_EACH(VECTOR, VAR)                                           \
    for (size_t _VECTOR_IDX_VAR = 0, _VECTOR_COUNT_VAR = (VECTOR)->len;        \
         _VECTOR_IDX_VAR == 0 && _VECTOR_COUNT_VAR > 0;)                       \
        for (void*(VAR) = vectorAt((VECTOR), 0);                               \
             ++_VECTOR_IDX_VAR,                                                \
            (_VECTOR_IDX_VAR - 1) < _VECTOR_COUNT_VAR &&                       \
                 ((VAR) = vectorAt((VECTOR), (_VECTOR_IDX_VAR - 1)), true);)
