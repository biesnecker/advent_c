#include "bitset.h"

#include <assert.h>

void bitsetInit(bitset* b, size_t capacity) {
    size_t c = (capacity >> 6) + 1;
    b->capacity = capacity;
    b->data = calloc(c, sizeof(uint64_t));
    assert(b->data != NULL);
}

void bitsetFinalize(bitset* b) {
    b->capacity = 0;
    free(b->data);
    b->data = NULL;
}

void bitsetSet(bitset* b, size_t idx, bool val) {
    assert(idx < b->capacity);
    size_t elem = idx / 64;
    size_t offset = idx % 64;
    uint64_t mask = 1ULL << offset;
    if (val) {
        b->data[elem] |= mask;
    } else {
        b->data[elem] &= (~mask);
    }
}

bool bitsetCheck(bitset* b, size_t idx) {
    assert(idx < b->capacity);
    size_t elem = idx / 64;
    size_t offset = idx % 64;
    uint64_t mask = 1ULL << offset;
    return (b->data[elem] & mask) != 0;
}
