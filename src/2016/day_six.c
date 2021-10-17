#include <stdint.h>

#include "../common.h"

#define COLCOUNT 8

typedef struct counts {
    uint32_t counts[COLCOUNT][26];
} counts;

static void
readInput(FILE* fp, void (*handler)(const char*, void*), void* userData) {
    char buffer[16];
    while (fscanf(fp, "%s\n", buffer) != EOF) {
        handler(buffer, userData);
    }
}

static void handleA(const char* s, void* userData) {
    counts* cnt = userData;
    for (int i = 0; i < COLCOUNT; ++i) {
        uint8_t idx = s[i] - 'a';
        cnt->counts[i][idx] += 1;
    }
}

FUNCTION_DEFN_FOR_YDS(2016, six, a) {
    counts cnts = {0};
    readInput(fp, handleA, &cnts);

    char key[COLCOUNT + 1] = {0};

    for (int i = 0; i < COLCOUNT; ++i) {
        uint32_t maxIdx = 0;
        uint32_t maxIdxCount = 0;
        for (int j = 0; j < 26; ++j) {
            uint32_t c = cnts.counts[i][j];
            if (c > maxIdxCount) {
                maxIdxCount = c;
                maxIdx = j;
            }
        }
        key[i] = (char)maxIdx + 'a';
    }
    printf("%s\n", key);
}

FUNCTION_DEFN_FOR_YDS(2016, six, b) {
    counts cnts = {0};
    readInput(fp, handleA, &cnts);

    char key[COLCOUNT + 1] = {0};

    for (int i = 0; i < COLCOUNT; ++i) {
        uint32_t minIdx = 0;
        uint32_t minIdxCount = UINT32_MAX;
        for (int j = 0; j < 26; ++j) {
            uint32_t c = cnts.counts[i][j];
            if (c < minIdxCount) {
                minIdxCount = c;
                minIdx = j;
            }
        }
        key[i] = (char)minIdx + 'a';
    }
    printf("%s\n", key);
}
