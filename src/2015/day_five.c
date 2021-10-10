#include <stdbool.h>
#include <string.h>

#include "../macros.h"

static bool isVowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

FUNCTION_DEFN_FOR_YDS(2015, five, a) {
    int good = 0;
    char line[100] = {0};
    char last;
    while (fgets(line, sizeof(line), fp) != NULL) {
        int vowelCount = 0;
        bool foundDouble = false;
        int len = strlen(line);
        last = line[0];
        if (isVowel(last)) {
            ++vowelCount;
        }

        bool foundBad = false;
        for (int i = 1; i < len - 1; ++i) {
            char next = line[i];
            if (isVowel(next)) {
                ++vowelCount;
            }
            if (next == last) {
                foundDouble = true;
            }
            switch (next) {
            case 'b':
                foundBad = last == 'a';
                break;
            case 'd':
                foundBad = last == 'c';
                break;
            case 'q':
                foundBad = last == 'p';
                break;
            case 'y':
                foundBad = last == 'x';
                break;
            }
            if (foundBad) {
                break;
            }
            last = next;
        }

        if (!foundBad && foundDouble && vowelCount > 2) {
            ++good;
        }
    }
    printf("%d\n", good);
}

FUNCTION_DEFN_FOR_YDS(2015, five, b) {
    int good = 0;
    char line[100] = {0};
    while (fgets(line, sizeof(line), fp) != NULL) {
        int len = strlen(line) - 1;

        bool foundAxA = false;

        // First let's look for the AxA pattern.
        int pos = 0;
        while (pos + 2 < len) {
            if (line[pos] == line[pos + 2]) {
                foundAxA = true;
                break;
            }
            ++pos;
        }

        if (!foundAxA) {
            continue;
        }

        bool foundDouble = false;

        // Find the non-overlapping doubles.
        pos = 0;
        while (pos + 3 < len) {
            char fst = line[pos];
            char snd = line[pos + 1];
            for (int i = pos + 2; i < len - 1; ++i) {
                if (line[i] == fst && line[i + 1] == snd) {
                    foundDouble = true;
                    break;
                }
            }
            if (foundDouble) {
                break;
            }
            ++pos;
        }
        if (foundDouble) {
            ++good;
        }
    }
    printf("%d\n", good);
}
