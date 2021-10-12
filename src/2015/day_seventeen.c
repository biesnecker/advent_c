#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#include "../macros.h"

#define NBUCKETS 20
#define GOAL 150

static void readInput(FILE* fp, int* buckets) {
    int val;
    for (int i = 0; i < NBUCKETS; ++i) {
        assert(fscanf(fp, "%d\n", &val) == 1);
        buckets[i] = val;
    }
}

typedef struct solution {
    int combinations;
    int minBuckets;
    int minBucketCount;
} solution;

void step(int* buckets, int total, int lastIdx, int bucketsUsed, solution* s) {
    assert(lastIdx + 1 >= 0);
    if (total == GOAL) {
        // If we've hit the goal then we're done.
        s->combinations += 1;

        if (bucketsUsed == s->minBuckets) {
            s->minBucketCount += 1;
        } else if (bucketsUsed < s->minBuckets) {
            s->minBuckets = bucketsUsed;
            s->minBucketCount = 1;
        }
        return;
    }
    if (total > GOAL) {
        // If we're over the goal then we're done.
        return;
    }
    if (lastIdx + 1 >= NBUCKETS) {
        // Checked all the buckets.
        return;
    }

    for (int i = lastIdx + 1; i < NBUCKETS; ++i) {
        step(buckets, total + buckets[i], i, bucketsUsed + 1, s);
    }
}

static solution getSolution(FILE* fp) {
    int buckets[NBUCKETS] = {0};
    readInput(fp, buckets);

    solution s = {
        .combinations = 0, .minBuckets = INT_MAX, .minBucketCount = 0};
    step(buckets, 0, -1, 0, &s);
    return s;
}

FUNCTION_DEFN_FOR_YDS(2015, seventeen, a) {
    solution s = getSolution(fp);

    printf("%d\n", s.combinations);
}

FUNCTION_DEFN_FOR_YDS(2015, seventeen, b) {
    solution s = getSolution(fp);

    printf("%d\n", s.minBucketCount);
}
