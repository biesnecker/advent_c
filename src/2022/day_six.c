#include "../common.h"

static uint32_t letterMask(char c) {
    return 1U << (c - 'a');
}

// Assumes that the message is at least as long as the marker length.
static int findMarker(const char* msg, int len) {
    for (int idx = len - 1; msg[idx] != '\n' && msg[idx] != '\0'; idx++) {
        uint32_t mask = 0;
        for (int i = idx - len + 1; i <= idx; i++) {
            mask |= letterMask(msg[i]);
        }
        if (__builtin_popcount(mask) == len) {
            return idx + 1;
        }
    }
    return -1;
}

FUNCTION_DEFN_FOR_YDS(2022, six, a) {
    char buffer[8192];
    assert(fgets(buffer, sizeof(buffer), fp) != NULL);
    printf("%d\n", findMarker(buffer, 4));
}

FUNCTION_DEFN_FOR_YDS(2022, six, b) {
    char buffer[8192];
    assert(fgets(buffer, sizeof(buffer), fp) != NULL);
    printf("%d\n", findMarker(buffer, 14));
}
