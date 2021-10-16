#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct bitset {
    size_t capacity;
    uint64_t* data;
} bitset;

void bitsetInit(bitset* b, size_t capacity);

void bitsetFinalize(bitset* b);

void bitsetSet(bitset* b, size_t idx, bool val);

bool bitsetCheck(bitset* b, size_t idx);
