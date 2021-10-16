#include "../common.h"

static void readInput(FILE* fp,
                      void (*handler)(uint32_t, uint32_t, uint32_t, void*),
                      void* userData) {
    uint32_t a, b, c;
    while (fscanf(fp, "%u%u%u", &a, &b, &c) != EOF) {
        handler(a, b, c, userData);
    }
}

static bool isPossible(uint32_t a, uint32_t b, uint32_t c) {
    return (a + b > c && a + c > b && b + c > a);
}

static void handlerA(uint32_t a, uint32_t b, uint32_t c, void* userData) {
    uint32_t* cnt = userData;
    if (isPossible(a, b, c)) {
        *cnt += 1;
    }
}

typedef struct state {
    uint32_t columnOne[3];
    uint32_t columnTwo[3];
    uint32_t columnThree[3];
    uint32_t idx;
    uint32_t cnt;
} state;

static void handlerB(uint32_t a, uint32_t b, uint32_t c, void* userData) {
    state* s = userData;
    s->columnOne[s->idx] = a;
    s->columnTwo[s->idx] = b;
    s->columnThree[s->idx] = c;
    if (s->idx < 2) {
        s->idx += 1;
    } else {
        if (isPossible(s->columnOne[0], s->columnOne[1], s->columnOne[2])) {
            s->cnt += 1;
        }
        if (isPossible(s->columnTwo[0], s->columnTwo[1], s->columnTwo[2])) {
            s->cnt += 1;
        }
        if (isPossible(
                s->columnThree[0], s->columnThree[1], s->columnThree[2])) {
            s->cnt += 1;
        }
        s->idx = 0;
    }
}

FUNCTION_DEFN_FOR_YDS(2016, three, a) {
    uint32_t cnt = 0;
    readInput(fp, handlerA, &cnt);
    printf("%u\n", cnt);
}

FUNCTION_DEFN_FOR_YDS(2016, three, b) {
    state s = {0};
    readInput(fp, handlerB, &s);
    assert(s.idx == 0);
    printf("%u\n", s.cnt);
}
