#include <stdint.h>

#include "../common.h"
#include "../utils/md5.h"
#include "../utils/strings.h"

#define MAX_DOOR_ID 32
#define KEY_LEN 8

static const char hex[] = "0123456789abcdef";

typedef struct state {
    char key[KEY_LEN + 1];
    uint8_t keyIdx;
} state;

static void readInput(FILE* fp,
                      bool (*handler)(uint32_t h, void* userData),
                      void* userData) {

    char doorId[32];
    assert(fgets(doorId, sizeof(doorId), fp) != NULL);
    stripNewline(doorId);

    uint32_t idx = 0;
    char keyBuffer[64];
    uint32_t hash[] = {0, 0, 0, 0};
    do {
        int len = sprintf(keyBuffer, "%s%u", doorId, idx++);
        assert(len > 0);
        md5((uint8_t*)keyBuffer, len, (uint8_t*)hash);
    } while (handler(hash[0], userData));
}

static bool handlerA(uint32_t h, void* userData) {
    state* s = userData;
    if ((h & 0xf0ffff) == 0) {
        uint32_t x = (h & 0xf0000U) >> 16;
        assert(x >= 0 && x < 16);
        s->key[s->keyIdx++] = hex[x];
    }
    return s->keyIdx < KEY_LEN;
}

static bool handlerB(uint32_t h, void* userData) {
    state* s = userData;
    if ((h & 0xf0ffffU) == 0) {
        uint32_t idx = (h & 0xf0000U) >> 16;
        uint32_t mask = 1U << idx;

        // If it's an invalid or already set position ignore it.
        if (idx < 0 || idx >= KEY_LEN || (s->keyIdx & mask) > 0) {
            return true;
        }

        uint32_t val = (h & 0xf0000000U) >> 28;
        assert(val >= 0 && val < 16);
        s->key[idx] = hex[val];
        s->keyIdx |= mask;
        return s->keyIdx != UINT8_MAX;
    }
    return true;
}

FUNCTION_DEFN_FOR_YDS(2016, five, a) {
    state s = {0};
    readInput(fp, handlerA, &s);
    printf("%s\n", s.key);
}

FUNCTION_DEFN_FOR_YDS(2016, five, b) {
    state s = {0};
    readInput(fp, handlerB, &s);
    printf("%s\n", s.key);
}
