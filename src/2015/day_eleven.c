#include "../common.h"

static inline void stepPassword(char* pw, int pwlen) {
    int currentIdx = pwlen - 1;
    while (true) {
        if (pw[currentIdx] == 'z') {
            assert(currentIdx > 0);
            pw[currentIdx] = 'a';
            --currentIdx;
        } else {
            char next = pw[currentIdx] + 1;
            if (next == 'i' || next == 'o' || next == 'l') {
                ++next;
            }
            pw[currentIdx] = next;

            currentIdx = pwlen - 1;
            break;
        }
    }
}

static bool hasPairs(char* pw, int pwlen) {
    char firstPair = '\0';
    for (int i = 0; i < pwlen - 1; ++i) {
        if (pw[i] == pw[i + 1]) {
            if (firstPair == '\0') {
                firstPair = pw[i];
                ++i; // skip the next character.
                continue;
            } else if (firstPair != pw[i]) {
                return true;
            }
        }
    }

    return false;
}

static bool hasRising(char* pw, int pwlen) {
    for (int i = 0; i < pwlen - 2; ++i) {
        if (pw[i] == (pw[i + 1] - 1) && pw[i] == (pw[i + 2] - 2)) {
            return true;
        }
    }
    return false;
}

FUNCTION_DEFN_FOR_YDS(2015, eleven, a) {
    char buffer[100] = {0};
    assert(fscanf(fp, "%s\n", buffer) == 1);
    int pwlen = strlen(buffer);
    while (true) {
        stepPassword(buffer, pwlen);
        if (hasPairs(buffer, pwlen) && hasRising(buffer, pwlen)) {
            printf("%s\n", buffer);
            break;
        }
    }
}

FUNCTION_DEFN_FOR_YDS(2015, eleven, b) {
    char buffer[100] = {0};
    assert(fscanf(fp, "%s\n", buffer) == 1);
    int pwlen = strlen(buffer);
    int found = 0;
    while (true) {
        stepPassword(buffer, pwlen);
        if (hasPairs(buffer, pwlen) && hasRising(buffer, pwlen)) {
            ++found;
            if (found == 2) {
                printf("%s\n", buffer);
                break;
            }
        }
    }
}
