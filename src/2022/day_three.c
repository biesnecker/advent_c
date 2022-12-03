#include "../common.h"

typedef struct {
    uint64_t partA;
    uint64_t partB;
} rucksack;

typedef struct {
    rucksack elves[3];
    int seen;
    int total;
} p2state;

static void
readInput(FILE* fp, void (*handler)(const char*, void*), void* userData) {
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        handler(buffer, userData);
    }
}

static uint64_t charToMask(char c) {
    assert(islower(c) || isupper(c));
    if (islower(c)) {
        return ((uint64_t)1) << (c - 'a');
    }
    // Uppercase
    return ((uint64_t)1) << (26 + (c - 'A'));
}

static char maskToChar(uint64_t m) {
    assert(__builtin_popcountll(m) == 1);
    int x = __builtin_ctzll(m);
    if (x < 26) {
        return 'a' + x;
    }
    return 'A' + (x - 26);
}

static int charToPriority(char c) {
    assert(islower(c) || isupper(c));
    if (islower(c)) {
        return c - 'a' + 1;
    }
    return c - 'A' + 27;
}

static rucksack stringToRucksack(const char* s) {
    rucksack r = {0};
    size_t slen = strlen(s) - 1; // subtract 1 for newline
    for (size_t i = 0; i < slen; i++) {
        if (i < slen / 2) {
            r.partA |= charToMask(s[i]);
        } else {
            r.partB |= charToMask(s[i]);
        }
    }
    return r;
}

static uint64_t getDuplicateMask(rucksack r) {
    return (r.partA & r.partB);
}

static uint64_t getCommonMask(rucksack* r) {
    uint64_t common = UINT64_MAX;
    for (int i = 0; i < 3; i++) {
        common &= (r[i].partA | r[i].partB);
    }
    return common;
}

static void handlerA(const char* s, void* userData) {
    int* prioritySum = (int*)userData;
    *prioritySum +=
        charToPriority(maskToChar(getDuplicateMask(stringToRucksack(s))));
}

static void handlerB(const char* s, void* userData) {
    p2state* state = (p2state*)userData;
    state->elves[state->seen] = stringToRucksack(s);
    state->seen += 1;

    if (state->seen == 3) {
        state->total += charToPriority(maskToChar(getCommonMask(state->elves)));
        state->seen = 0;
    }
}

FUNCTION_DEFN_FOR_YDS(2022, three, a) {
    int prioritySum = 0;
    readInput(fp, handlerA, &prioritySum);
    printf("%d\n", prioritySum);
}

FUNCTION_DEFN_FOR_YDS(2022, three, b) {
    p2state s = {0};
    readInput(fp, handlerB, &s);
    printf("%d\n", s.total);
}
