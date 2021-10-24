#include <string.h>

#include "../common.h"
#include "../utils/md5.h"
#include "../utils/ringbuffer.h"
#include "../utils/strings.h"

typedef struct hashres {
    char* hash;
    size_t idx;
    char three;
    char five;
} hashres;

static void freeHashres(void* hr) {
    hashres* h = hr;
    free(h->hash);
}

static char* readInput(FILE* fp) {
    char buffer[64];
    assert(fgets(buffer, sizeof(buffer), fp) != NULL);
    stripNewline(buffer);
    return strdup(buffer);
}

static char hasThree(const char* s, size_t len) {
    if (len < 3) {
        return 0;
    }
    for (size_t i = 0; i < len - 2; ++i) {
        if (s[i] == s[i + 1] && s[i] == s[i + 2]) {
            return s[i];
        }
    }
    return 0;
}

static char hasFive(const char* s, size_t len) {
    if (len < 5) {
        return 0;
    }
    for (size_t i = 0; i < len - 4; ++i) {
        if (s[i] == s[i + 1] && s[i] == s[i + 2] && s[i] == s[i + 3] &&
            s[i] == s[i + 4]) {
            return s[i];
        }
    }
    return 0;
}

static char* stringHash(const char* s, bool stretch) {
    if (stretch) {
        char buffer[33];
        md5HexToBuffer(s, strlen(s), buffer);
        for (int i = 0; i < 2016; ++i) {
            md5HexToBuffer(buffer, 32, buffer);
        }
        return strdup(buffer);
    } else {
        return md5Hex(s, strlen(s));
    }
}

static size_t findSolution(char* salt, bool stretch) {
    char buffer[64] = {0};

    size_t found = 0;
    size_t foundIdx = 0;
    size_t idx = 0;

    ringbuffer r;
    ringbufferInit(&r, 1 << 10, sizeof(hashres), freeHashres);

    while (found < 64) {
        snprintf(buffer, sizeof(buffer), "%s%zu", salt, idx);
        char* bufferHash = stringHash(buffer, stretch);
        char three = hasThree(bufferHash, 32);
        char five = hasFive(bufferHash, 32);
        if (three != 0 || five != 0) {
            hashres hr = {
                .hash = bufferHash, .idx = idx, .three = three, .five = five};
            ringbufferPush(&r, &hr);
            if (five != 0) {
                // We added a five to the list, maybe we can clear up a three?
                while (ringbufferSize(&r) > 1) {
                    hashres* tail = ringbufferAt(&r, 0);
                    assert(tail->three != 0);
                    if (tail->idx + 1000 > idx) {
                        // Don't bother processing until we've seen enough to be
                        // sure about this element.
                        break;
                    }

                    for (size_t i = 1; i < ringbufferSize(&r); ++i) {
                        hashres* other = ringbufferAt(&r, i);
                        if (tail->three == other->five &&
                            tail->idx + 1000 >= other->idx && found < 64) {
                            found += 1;
                            foundIdx = tail->idx;
                        }
                    }
                    ringbufferPop(&r);
                }
            }
        } else {
            free(bufferHash);
        }
        ++idx;
    }

    ringbufferFinalize(&r);
    return foundIdx;
}

FUNCTION_DEFN_FOR_YDS(2016, fourteen, a) {
    char* salt = readInput(fp);
    printf("%zu\n", findSolution(salt, false));
    free(salt);
}

FUNCTION_DEFN_FOR_YDS(2016, fourteen, b) {
    char* salt = readInput(fp);
    printf("%zu\n", findSolution(salt, true));
    free(salt);
}
