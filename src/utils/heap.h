#pragma once

#include "vector.h"

typedef struct heap {
    vector v;
    int (*cmp)(const void*, const void*);
} heap;

void heapInit(heap* h,
              size_t capacity,
              size_t elemSize,
              int (*cmp)(const void*, const void*));

void heapFinalize(heap* h);

size_t heapSize(const heap* h);

size_t heapCapacity(const heap* h);

void* heapPeek(const heap* h);

void heapPop(heap* h);

void heapPush(heap* h, const void* elem);
