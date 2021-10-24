#pragma once

// Pure C implementation of Yann Collet's xxHash, heavily inspired by
// https://create.stephan-brumme.com/xxhash/.

#include <stdint.h>
#include <stdlib.h>

#define _XXHASH_MAX_BUFFER_SIZE 32

typedef struct xxhash {
    uint64_t state[4];
    uint8_t buffer[_XXHASH_MAX_BUFFER_SIZE];
    uint32_t bufferSize;
    uint32_t totalLength;
} xxhash;

void xxhashInit(xxhash* h, uint64_t seed);

void xxhashAppend(xxhash* h, const void* input, size_t len);

uint64_t xxhashDigest(const xxhash* h);

uint64_t xxhashFromString(const char* s);

uint64_t xxhashFromData(const void* s, size_t len);
