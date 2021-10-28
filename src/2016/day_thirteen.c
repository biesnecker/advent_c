#include <x86intrin.h>

#include "../common.h"
#include "../utils/hashset.h"
#include "../utils/heap.h"
#include "../utils/xxhash.h"

typedef struct location {
    int x, y;
} location;

static int compareLocation(const void* a, const void* b) {
    const location* m = a;
    const location* n = b;
    if (m->x != n->x) {
        return m->x - n->x;
    } else {
        return m->y - n->y;
    }
}

static uint64_t hashLocation(const void* a) {
    const location* loc = a;
    xxhash hsh;
    xxhashInit(&hsh, 0);
    xxhashAppend(&hsh, &loc->x, sizeof(int));
    xxhashAppend(&hsh, &loc->y, sizeof(int));
    return xxhashDigest(&hsh);
}

typedef struct pos {
    location loc;
    int steps;
} pos;

static int comparePos(const void* a, const void* b) {
    const pos* p = a;
    const pos* q = b;
    if (p->steps != q->steps) {
        return p->steps - q->steps;
    } else {
        return compareLocation(&p->loc, &q->loc);
    }
}

static int readInput(FILE* fp) {
    int salt;
    assert(fscanf(fp, "%d", &salt) == 1);
    return salt;
}

static bool isWall(const location* loc, int salt) {
    int v = (loc->x * loc->x) + (3 * loc->x) + (2 * loc->x * loc->y) +
            (loc->y + loc->y * loc->y);
    v += salt;
    return __popcntd(v) % 2 == 1;
}

FUNCTION_DEFN_FOR_YDS(2016, thirteen, a) {
    int salt = readInput(fp);
    location target = {.x = 31, .y = 39};

    heap pqueue;
    heapInit(&pqueue, 1 << 10, sizeof(pos), comparePos, NULL);

    int shortest = INT_MAX;

    hashset seen;
    hashsetInit(
        &seen, 1 << 10, sizeof(location), compareLocation, hashLocation, NULL);

    pos startingPosition = {.loc = {.x = 1, .y = 1}, .steps = 0};
    heapPush(&pqueue, &startingPosition);

    pos* p;

    while ((p = heapPeek(&pqueue)) != NULL) {
        int newSteps = p->steps + 1;
        location loc = p->loc;
        heapPop(&pqueue);

        if (newSteps >= shortest) {
            continue;
        }

        bool isNew;
        hashsetLookupOrInsert(&seen, &loc, &isNew);
        if (!isNew) {
            continue;
        }

        for (int dir = 0; dir < 2; ++dir) {
            for (int step = -1; step < 2; ++step) {
                if (step == 0) {
                    continue;
                }
                location newLoc = {.x = loc.x, .y = loc.y};
                if (dir == 0) {
                    newLoc.x += step;
                } else {
                    newLoc.y += step;
                }
                if (newLoc.x < 0 || newLoc.y < 0) {
                    continue;
                }
                if (hashsetContains(&seen, &newLoc)) {
                    continue;
                }
                if (compareLocation(&newLoc, &target) == 0) {
                    if (newSteps < shortest) {
                        shortest = newSteps;
                        continue;
                    }
                }
                if (isWall(&newLoc, salt)) {
                    continue;
                }
                pos newPos = {.loc = newLoc, .steps = newSteps};
                heapPush(&pqueue, &newPos);
            }
        }
    }

    printf("%d\n", shortest);

    hashsetFinalize(&seen);
    heapFinalize(&pqueue);
}

FUNCTION_DEFN_FOR_YDS(2016, thirteen, b) {
    int count = 0;
    int salt = readInput(fp);

    heap pqueue;
    heapInit(&pqueue, 1 << 10, sizeof(pos), comparePos, NULL);

    hashset seen;
    hashsetInit(
        &seen, 1 << 10, sizeof(location), compareLocation, hashLocation, NULL);

    pos startingPosition = {.loc = {.x = 1, .y = 1}, .steps = 0};
    heapPush(&pqueue, &startingPosition);

    pos* p;

    while ((p = heapPeek(&pqueue)) != NULL) {
        int newSteps = p->steps + 1;
        location loc = p->loc;
        heapPop(&pqueue);

        bool isNew;
        hashsetLookupOrInsert(&seen, &loc, &isNew);
        if (!isNew) {
            continue;
        }
        if (p->steps <= 50) {
            count += 1;
        }
        if (p->steps >= 50) {
            continue;
        }

        for (int dir = 0; dir < 2; ++dir) {
            for (int step = -1; step < 2; ++step) {
                if (step == 0) {
                    continue;
                }
                location newLoc = {.x = loc.x, .y = loc.y};
                if (dir == 0) {
                    newLoc.x += step;
                } else {
                    newLoc.y += step;
                }
                if (newLoc.x < 0 || newLoc.y < 0) {
                    continue;
                }
                if (isWall(&newLoc, salt)) {
                    continue;
                }
                if (hashsetContains(&seen, &newLoc)) {
                    continue;
                }
                pos newPos = {.loc = newLoc, .steps = newSteps};
                heapPush(&pqueue, &newPos);
            }
        }
    }

    printf("%d\n", count);

    hashsetFinalize(&seen);
    heapFinalize(&pqueue);
}
