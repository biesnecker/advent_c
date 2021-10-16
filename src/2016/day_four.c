#include <ctype.h>

#include "../common.h"

typedef struct charcount {
    char c;
    uint8_t cnt;
} charcount;

static int compareCharcount(const void* a, const void* b) {
    const charcount* x = a;
    const charcount* y = b;
    if (x->cnt == y->cnt) {
        return x->c - y->c;
    } else if (x->cnt < y->cnt) {
        return 1;
    } else {
        return -1;
    }
}

static void readInput(FILE* fp,
                      bool (*handler)(const char* roomName,
                                      const char* sectorId,
                                      const char* checkSum,
                                      void* userData),
                      void* userData) {
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        char *sectorId, *checkSum;
        char* p = buffer;
        int mode = 0;
        while (*p != '\0') {
            char c = *p;
            if (mode == 0 && isdigit(c)) {
                // Find the first digit, then insert a NUL into the dash
                // before it.
                p[-1] = '\0';
                sectorId = p;
                ++mode;
            } else if (mode == 1 && c == '[') {
                // NUL out the character before the checksum.
                checkSum = p + 1;
                *p = '\0';
                ++mode;
            } else if (mode == 2 && c == ']') {
                *p = '\0';
            }
            ++p;
        }
        if (!handler(buffer, sectorId, checkSum, userData)) {
            break;
        }
    }
}

static bool handlerA(const char* roomName,
                     const char* sectorId,
                     const char* checkSum,
                     void* userData) {
    uint32_t* total = userData;
    charcount counts[26] = {
        {'a', 0}, {'b', 0}, {'c', 0}, {'d', 0}, {'e', 0}, {'f', 0}, {'g', 0},
        {'h', 0}, {'i', 0}, {'j', 0}, {'k', 0}, {'l', 0}, {'m', 0}, {'n', 0},
        {'o', 0}, {'p', 0}, {'q', 0}, {'r', 0}, {'s', 0}, {'t', 0}, {'u', 0},
        {'v', 0}, {'w', 0}, {'x', 0}, {'y', 0}, {'z', 0}};

    while (*roomName != '\0') {
        char c = *roomName++;
        if (c != '-') {
            counts[c - 'a'].cnt += 1;
        }
    }

    qsort(counts, 26, sizeof(charcount), compareCharcount);
    bool legit = true;
    for (int i = 0; i < 5; ++i) {
        if (counts[i].c != checkSum[i]) {
            legit = false;
            break;
        }
    }
    if (legit) {
        uint32_t n;
        sscanf(sectorId, "%u", &n);
        *total += n;
    }
    return true;
}

static bool handlerB(const char* roomName,
                     const char* sectorId,
                     const char* checkSum,
                     void* userData) {
    UNUSED_ARG(checkSum);
    char* name = strdup(roomName);
    char* p = name;
    uint32_t shift;
    sscanf(sectorId, "%u", &shift);
    while (*p != '\0') {
        if (*p == '-') {
            *p = ' ';
        } else {
            uint32_t letter = *p - 'a';
            letter += shift;
            letter %= 26;
            letter += 'a';
            assert(letter >= 'a' && letter <= 'z');
            *p = (char)letter;
        }
        ++p;
    }
    bool finished = false;
    if (strstr(name, "north") != NULL) {
        finished = true;
        *(uint32_t*)userData = shift;
    }
    free(name);
    return !finished;
}

FUNCTION_DEFN_FOR_YDS(2016, four, a) {
    uint32_t solution = 0;
    readInput(fp, handlerA, &solution);
    printf("%u\n", solution);
}

FUNCTION_DEFN_FOR_YDS(2016, four, b) {
    uint32_t solution = 0;
    readInput(fp, handlerB, &solution);
    assert(solution != 0);
    printf("%u\n", solution);
}
