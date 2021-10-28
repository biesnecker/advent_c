#include "heap.h"

#include <assert.h>
#include <string.h>

#include "vector.h"

void heapInit(heap* h,
              size_t capacity,
              size_t elemSize,
              int (*cmp)(const void*, const void*),
              void (*free)(void*)) {
    vectorInit(&h->v, capacity, elemSize, free);
    h->cmp = cmp;
}

void heapFinalize(heap* h) {
    vectorFinalize(&h->v);
}

size_t heapSize(const heap* h) {
    return VECTOR_SIZE(&h->v);
}

size_t heapCapacity(const heap* h) {
    return VECTOR_CAPACITY(&h->v);
}

void* heapPeek(const heap* h) {
    if (VECTOR_SIZE(&h->v) == 0) {
        return NULL;
    }
    return VECTOR_AT(&h->v, 0);
}

static void _siftdown(heap* h, size_t startPos, size_t pos) {
    size_t elemSize = h->v.elemSize;
    uint8_t tmp[elemSize];
    vector* v = &h->v;
    void* newItem = VECTOR_AT(v, pos);
    while (pos > startPos) {
        size_t parentPos = (pos - 1) / 2;
        void* parent = VECTOR_AT(v, parentPos);
        int cmp = h->cmp(newItem, parent);
        if (cmp >= 0) {
            break; // They're equal or greater so keep the element here.
        }
        memcpy(tmp, newItem, elemSize);
        memcpy(newItem, parent, elemSize);
        memcpy(parent, tmp, elemSize);
        pos = parentPos;
        newItem = parent;
    }
}

static void _siftup(heap* h, size_t pos) {
    vector* v = &h->v;
    size_t elemSize = v->elemSize;
    size_t endPos = v->len;
    size_t startPos = pos;
    size_t limit = endPos / 2;
    uint8_t tmp[elemSize];
    void* elem = VECTOR_AT(v, pos);
    while (pos < limit) {
        size_t childPos = 2 * pos + 1; // leftmost child
        void* childOne = VECTOR_AT(v, childPos);
        void* childTwo = NULL;
        if (childPos + 1 < endPos) {
            childTwo = VECTOR_AT(v, childPos + 1);
            int cmp = h->cmp(childOne, childTwo);
            if (cmp >= 0) {
                ++childPos;
                childOne = childTwo;
            }
        }
        memcpy(tmp, childOne, elemSize);
        memcpy(childOne, elem, elemSize);
        memcpy(elem, tmp, elemSize);

        pos = childPos;
        elem = childOne;
    }
    _siftdown(h, startPos, pos);
}

void heapPop(heap* h) {
    size_t hs = heapSize(h);
    assert(hs > 0);

    if (hs == 1) {
        vectorTruncate(&h->v);
        return;
    }

    void* last = VECTOR_BACK(&h->v);
    void* first = VECTOR_AT(&h->v, 0);

    // Copy the last element to the front.
    memcpy(first, last, h->v.elemSize);
    vectorPopBack(&h->v);

    _siftup(h, 0);
}

void heapPush(heap* h, const void* elem) {
    vectorPush(&h->v, elem);
    _siftdown(h, 0, vectorSize(&h->v) - 1);
}
