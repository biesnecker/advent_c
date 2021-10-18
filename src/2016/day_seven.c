#include <stdio.h>
#include <string.h>

#include "../common.h"

typedef struct segments {
    char* normal[10];
    int normalLens[10];
    char* special[10];
    int specialLens[10];
    int normalIdx;
    int specialIdx;
} segments;

static bool hasABBA(char* s, int len) {
    if (len < 4) {
        return false;
    }
    for (int i = 0; i < len - 3; ++i) {
        if (s[i] != s[i + 1] && s[i] == s[i + 3] && s[i + 1] == s[i + 2]) {
            return true;
        }
    }
    return false;
}

static bool hasBAB(char* s, int len, char a, char b) {
    if (len < 3) {
        return false;
    }
    for (int i = 0; i < len - 2; ++i) {
        if (s[i] == b && s[i + 1] == a && s[i + 2] == b) {
            return true;
        }
    }
    return false;
}

static void
readInput(FILE* fp, void (*handler)(segments*, void*), void* userData) {
    char buffer[256];
    while (fscanf(fp, "%s\n", buffer) != EOF) {
        segments s = {.normal = {buffer, NULL, NULL},
                      .normalIdx = 1,
                      .special = {NULL, NULL, NULL},
                      .specialIdx = 0};

        while (true) {
            char* start = strchr(s.normal[s.normalIdx - 1], '[');
            if (start == NULL) {
                break;
            }
            *start = '\0';
            start++;
            s.special[s.specialIdx++] = start;

            char* end = strchr(start, ']');
            assert(end != NULL);
            *end = '\0';
            s.normal[s.normalIdx++] = end + 1;
        }

        for (int i = 0; i < s.normalIdx; ++i) {
            s.normalLens[i] = strlen(s.normal[i]);
        }
        for (int i = 0; i < s.specialIdx; ++i) {
            s.specialLens[i] = strlen(s.special[i]);
        }

        handler(&s, userData);
    }
}

static void handlerA(segments* s, void* userData) {
    uint32_t* total = userData;
    bool hasAllowedAbba = false;
    bool hasDisallowedAbba = false;

    for (int i = 0; i < s->normalIdx; ++i) {
        if (hasABBA(s->normal[i], s->normalLens[i])) {
            hasAllowedAbba = true;
            break;
        }
    }

    for (int i = 0; i < s->specialIdx; ++i) {
        if (hasABBA(s->special[i], s->specialLens[i])) {
            hasDisallowedAbba = true;
            break;
        }
    }

    if (hasAllowedAbba && !hasDisallowedAbba) {
        *total += 1;
    }
}

static void handlerB(segments* s, void* userData) {
    uint32_t* total = userData;

    bool found = false;

    for (int i = 0; i < s->normalIdx; ++i) {
        for (int j = 0; j < s->normalLens[i] - 2; j++) {
            if (s->normal[i][j] != s->normal[i][j + 1] &&
                s->normal[i][j] == s->normal[i][j + 2]) {
                char a = s->normal[i][j];
                char b = s->normal[i][j + 1];
                for (int k = 0; k < s->specialIdx; ++k) {
                    if (hasBAB(s->special[k], s->specialLens[k], a, b)) {
                        found = true;
                        goto loopComplete;
                    }
                }
            }
        }
    }
loopComplete:
    if (found) {
        *total += 1;
    }
}

FUNCTION_DEFN_FOR_YDS(2016, seven, a) {
    uint32_t solution = 0;
    readInput(fp, handlerA, &solution);
    printf("%u\n", solution);
}

FUNCTION_DEFN_FOR_YDS(2016, seven, b) {
    uint32_t solution = 0;
    readInput(fp, handlerB, &solution);
    printf("%u\n", solution);
}
