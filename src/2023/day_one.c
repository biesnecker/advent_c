#include <ctype.h>

#include "../common.h"

static void
readInput(FILE* fp, void (*handler)(const char*, void*), void* userData) {
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        handler(buffer, userData);
    }
}

#define SET_FIRST_LAST(n)                                                      \
    {                                                                          \
        if (first < 0) {                                                       \
            first = n;                                                         \
        }                                                                      \
        last = n;                                                              \
    }                                                                          \
    while (0)

static void handlerA(const char* buffer, void* userData) {
    int* s = (int*)userData;
    const char* p = buffer;
    int first = -1;
    int last = -1;
    while (*p != '\0') {
        if (isdigit(*p)) {
            SET_FIRST_LAST(*p - '0');
        }
        ++p;
    }
    assert(first > 0 && last > 0);
    *s += (first * 10 + last);
}

static void handlerB(const char* buffer, void* userData) {
    int* s = (int*)userData;
    int slen = strlen(buffer);
    int first = -1;
    int last = -1;

    for (int i = 0; i < slen; ++i) {
        if (isdigit(buffer[i])) {
            SET_FIRST_LAST(buffer[i] - '0');
        } else if (buffer[i] == 'o' && i <= slen - 3) {
            if (memcmp(buffer + i, "one", 3) == 0) {
                SET_FIRST_LAST(1);
            }
        } else if (buffer[i] == 't' && i <= slen - 3 &&
                   memcmp(buffer + i, "two", 3) == 0) {
            SET_FIRST_LAST(2);
        } else if (buffer[i] == 't' && i <= slen - 5 &&
                   memcmp(buffer + i, "three", 5) == 0) {
            SET_FIRST_LAST(3);
        } else if (buffer[i] == 'f' && i <= slen - 4 &&
                   memcmp(buffer + i, "four", 4) == 0) {
            SET_FIRST_LAST(4);
        } else if (buffer[i] == 'f' && i <= slen - 4 &&
                   memcmp(buffer + i, "five", 4) == 0) {
            SET_FIRST_LAST(5);
        } else if (buffer[i] == 's' && i <= slen - 3 &&
                   memcmp(buffer + i, "six", 3) == 0) {
            SET_FIRST_LAST(6);
        } else if (buffer[i] == 's' && i <= slen - 5 &&
                   memcmp(buffer + i, "seven", 5) == 0) {
            SET_FIRST_LAST(7);
        } else if (buffer[i] == 'e' && i <= slen - 5 &&
                   memcmp(buffer + i, "eight", 5) == 0) {
            SET_FIRST_LAST(8);
        } else if (buffer[i] == 'n' && i <= slen - 4 &&
                   memcmp(buffer + i, "nine", 4) == 0) {
            SET_FIRST_LAST(9);
        }
    }

    assert(first >= 0 && last >= 0);
    *s += (first * 10 + last);
}

FUNCTION_DEFN_FOR_YDS(2023, one, a) {
    int s = 0;
    readInput(fp, handlerA, &s);
    printf("%d\n", s);
}

FUNCTION_DEFN_FOR_YDS(2023, one, b) {
    int s = 0;
    readInput(fp, handlerB, &s);
    printf("%d\n", s);
}
