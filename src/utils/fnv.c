#include "fnv.h"

#include <string.h>

#define FNV_PRIME_32 16777619U
#define FNV_OFFSET_32 2166136261U

uint32_t fnv32(const void* data, size_t len) {
    uint8_t* bytes = (uint8_t*)data;
    uint32_t hash = FNV_OFFSET_32;
    for (size_t i = 0; i < len; ++i) {
        hash ^= bytes[i];
        hash *= FNV_PRIME_32;
    }
    return hash;
}

uint32_t fnv32_char(const char* s) {
    return fnv32(s, strlen(s));
}
