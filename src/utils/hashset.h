#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef int (*hashsetComp)(const void*, const void*);
typedef uint64_t (*hashsetHash)(const void*);
typedef void (*hashsetFree)(void*);

typedef struct hashset {
    size_t len;
    size_t capacity;
    size_t elemSize;
    size_t stride;
    void* data;
    hashsetComp comp;
    hashsetHash hash;
    hashsetFree free;
} hashset;

void hashsetInit(hashset* h,
                 size_t capacity,
                 size_t elemSize,
                 hashsetComp comp,
                 hashsetHash hash,
                 hashsetFree free);

void hashsetFinalize(hashset* h);

size_t hashsetSize(const hashset* h);

size_t hashsetCapacity(const hashset* h);

bool hashsetContains(hashset* h, const void* elem);

void* hashsetLookup(hashset* h, const void* elem);

void* hashsetInsert(hashset* h, const void* elem);

void* hashsetLookupOrInsert(hashset* h, const void* elem, bool* didInsert);
