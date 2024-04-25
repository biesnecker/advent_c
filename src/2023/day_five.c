#include <stdint.h>

#include "../common.h"

#define MAX_SEEDS 20
#define MAX_MAPPINGS 64
#define MAX_CATEGORIES 7
#define MAX_RANGES 64

typedef struct _Range {
    uint32_t start;
    uint32_t size;
} Range;

typedef struct _Mapping {
    uint32_t src;
    uint32_t dst;
    uint32_t size;
} Mapping;

typedef struct _Almanac {
    Mapping mappings[MAX_CATEGORIES][MAX_MAPPINGS];
    int mappingCount[MAX_CATEGORIES];
    uint32_t seeds[MAX_SEEDS];
    int seedCount;
    int nMappings;
} Almanac;

static int cmpRange(const void* a, const void* b) {
    const Range* ra = (const Range*)a;
    const Range* rb = (const Range*)b;

    if (ra->start < rb->start) {
        return -1;
    } else if (ra->start > rb->start) {
        return 1;
    }
    return 0;
}

static int cmpMapping(const void* a, const void* b) {
    const Mapping* ma = (const Mapping*)a;
    const Mapping* mb = (const Mapping*)b;

    if (ma->src < mb->src) {
        return -1;
    } else if (ma->src > mb->src) {
        return 1;
    }
    return 0;
}

static void readInput(FILE* fp, Almanac* almanac) {
    char buffer[512];
    int mode = -1;
    int header = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (buffer[0] == '\n') {
            mode++;
            header = 0;
            continue;
        }
        if (mode == -1) {
            char* saveptr;
            char* token = strtok_r(buffer, " ", &saveptr);
            while (token != NULL) {
                if (isnumber(*token)) {
                    almanac->seeds[almanac->seedCount++] =
                        strtoul(token, NULL, 10);
                }
                token = strtok_r(NULL, " ", &saveptr);
            }
        } else {
            assert(mode < MAX_CATEGORIES);
            if (header == 0) {
                almanac->nMappings++;
                header = 1;
                continue;
            }
            Mapping* mapping =
                &almanac->mappings[mode][almanac->mappingCount[mode]++];
            if (sscanf(
                    buffer,
                    "%u %u %u",
                    &mapping->dst,
                    &mapping->src,
                    &mapping->size
                ) != 3) {
                fprintf(stderr, "Failed to parse input\n");
                exit(1);
            }
        }
    }
    // Sort so we can binary search later.
    for (int i = 0; i < almanac->nMappings; i++) {
        qsort(
            almanac->mappings[i],
            almanac->mappingCount[i],
            sizeof(Mapping),
            cmpMapping
        );
    }
}

static uint32_t
map_value(const Mapping* mappings, int mappingCount, uint32_t value) {
    int i = 0;
    int j = mappingCount - 1;
    while (i <= j) {
        int m = (i + j) / 2;
        if (mappings[m].src <= value &&
            value < mappings[m].src + mappings[m].size) {
            return mappings[m].dst + (value - mappings[m].src);
        }
        if (value < mappings[m].src) {
            j = m - 1;
        } else {
            i = m + 1;
        }
    }
    return value;
}

FUNCTION_DEFN_FOR_YDS(2023, five, a) {
    Almanac almanac = {0};
    readInput(fp, &almanac);

    uint32_t res = UINT32_MAX;
    for (int i = 0; i < almanac.seedCount; ++i) {
        uint32_t v = almanac.seeds[i];
        for (int j = 0; j < almanac.nMappings; ++j) {
            v = map_value(almanac.mappings[j], almanac.mappingCount[j], v);
        }
        res = v < res ? v : res;
    }
    printf("%u\n", res);
}

FUNCTION_DEFN_FOR_YDS(2023, five, b) {
    UNUSED_ARG(fp);
}
