#include "xxhash.h"

#include <string.h>

// Magic!
static const uint64_t prime1 = 11400714785074694791ULL;
static const uint64_t prime2 = 14029467366897019727ULL;
static const uint64_t prime3 = 1609587929392839161ULL;
static const uint64_t prime4 = 9650029242287828579ULL;
static const uint64_t prime5 = 2870177450012600261ULL;

void xxhashInit(xxhash* h, uint64_t seed) {
    h->state[0] = seed + prime1 + prime2;
    h->state[1] = seed + prime2;
    h->state[2] = seed;
    h->state[3] = seed - prime1;
    h->bufferSize = 0;
    h->totalLength = 0;
    memset(h->buffer, 0, _XXHASH_MAX_BUFFER_SIZE);
}

static inline uint64_t _xxhashRotateLeft(uint64_t x, uint8_t bits) {
    return (x << bits) | (x >> (64 - bits));
}

static inline uint64_t _xxhashProcessSingle(uint64_t state, uint64_t input) {
    return _xxhashRotateLeft(state + input * prime2, 31) * prime1;
}

static inline void _xxhashProcess(xxhash* h, const void* data) {
    const uint64_t* block = (const uint64_t*)data;

    h->state[0] = _xxhashProcessSingle(h->state[0], block[0]);
    h->state[1] = _xxhashProcessSingle(h->state[1], block[1]);
    h->state[2] = _xxhashProcessSingle(h->state[2], block[2]);
    h->state[3] = _xxhashProcessSingle(h->state[3], block[3]);
}

void xxhashAppend(xxhash* h, const void* input, size_t len) {
    if (input == NULL || len == 0) {
        return;
    }

    h->totalLength += len;
    const uint8_t* data = (const uint8_t*)input;

    // If the buffer can fit the data, just copy it into the buffer.
    if (h->bufferSize + len < _XXHASH_MAX_BUFFER_SIZE) {
        while (len-- > 0) {
            h->buffer[h->bufferSize++] = *data++;
        }
        return;
    }

    const uint8_t* stop = data + len;
    const uint8_t* stopBlock = stop - _XXHASH_MAX_BUFFER_SIZE;

    // If there's already data in the buffer, fill it first, process it, then
    // handle the rest.
    if (h->bufferSize > 0) {
        while (h->bufferSize < _XXHASH_MAX_BUFFER_SIZE) {
            h->buffer[h->bufferSize++] = *data++;
        }
        _xxhashProcess(h, h->buffer);
    }

    // Process the data a block at a time.
    while (data <= stopBlock) {
        _xxhashProcess(h, data);
        data += 32;
    }

    // Copy whatever is left into the buffer.
    h->bufferSize = stop - data;
    for (uint32_t i = 0; i < h->bufferSize; ++i) {
        h->buffer[i] = data[i];
    }
}

uint64_t xxhashDigest(const xxhash* h) {
    uint64_t result;

    if (h->totalLength >= _XXHASH_MAX_BUFFER_SIZE) {
        result = _xxhashRotateLeft(h->state[0], 1) +
                 _xxhashRotateLeft(h->state[1], 7) +
                 _xxhashRotateLeft(h->state[2], 12) +
                 _xxhashRotateLeft(h->state[3], 18);
        result =
            (result ^ _xxhashProcessSingle(0, h->state[0])) * prime1 + prime4;
        result =
            (result ^ _xxhashProcessSingle(0, h->state[1])) * prime1 + prime4;
        result =
            (result ^ _xxhashProcessSingle(0, h->state[2])) * prime1 + prime4;
        result =
            (result ^ _xxhashProcessSingle(0, h->state[3])) * prime1 + prime4;
    } else {
        // Append never updated the internal state so original seed is still in
        // state[2].
        result = h->state[2] + prime5;
    }

    result += h->totalLength;

    // Process what's left in the buffer.
    const uint8_t* data = h->buffer;
    const uint8_t* stop = data + h->bufferSize;

    // Process eight bytes at a time.
    while (data + 8 <= stop) {
        result = _xxhashRotateLeft(
                     result ^ _xxhashProcessSingle(0, *(uint64_t*)data), 27) *
                     prime1 +
                 prime4;
        data += 8;
    }

    // If there are still at least four bytes then process those.
    if (data + 4 <= stop) {
        result = _xxhashRotateLeft(result ^ (*(uint32_t*)data) * prime1, 23) *
                     prime2 +
                 prime3;
        data += 4;
    }

    // Process whatever is left a byte at a time.
    while (data != stop) {
        result = _xxhashRotateLeft(result ^ (*data++) * prime5, 11) * prime1;
    }

    // Mix the bits
    result ^= result >> 33;
    result *= prime2;
    result ^= result >> 29;
    result *= prime3;
    result ^= result >> 32;
    return result;
}

uint64_t xxhashFromString(const char* s) {
    return xxhashFromData(s, strlen(s));
}

uint64_t xxhashFromData(const void* s, size_t len) {
    xxhash hsh;
    xxhashInit(&hsh, 0);
    xxhashAppend(&hsh, s, len);
    return xxhashDigest(&hsh);
}
