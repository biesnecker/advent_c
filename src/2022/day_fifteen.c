#include "../common.h"
#include "../utils/stack.h"

typedef struct {
    int begin, end;
} range;

STACK_DEFINE(range_stack, range, 32);

typedef struct {
    int x, y, r;
} sensor;

static int cmp(const void* a, const void* b) {
    const range* x = (const range*)a;
    const range* y = (const range*)b;

    return x->begin == y->begin ? 0 : x->begin < y->begin ? -1 : 1;
}

static int md(int x, int y, int a, int b) {
    return abs(x - a) + abs(y - b);
}

static range coveredRangeAtRow(const sensor* s, int row) {
    range rng = {.begin = INT_MIN, .end = INT_MAX};
    int diff = abs(row - s->y);
    if (diff <= s->r) {
        rng.begin = s->x - (s->r - diff);
        rng.end = s->x + (s->r - diff);
    }
    return rng;
}

static int readInput(FILE* fp, sensor* sensors) {
    char buffer[500];
    int i = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        int sx, sy, bx, by;
        assert(sscanf(buffer,
                      "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
                      &sx,
                      &sy,
                      &bx,
                      &by) == 4);
        sensors[i++] = (sensor){.x = sx, .y = sy, .r = md(sx, sy, bx, by)};
    }
    return i;
}

FUNCTION_DEFN_FOR_YDS(2022, fifteen, a) {
    sensor sensors[32] = {0};
    range ranges[32] = {0};
    const int nSensors = readInput(fp, sensors);
    int nRanges = 0;

    for (int i = 0; i < nSensors; ++i) {
        range r = coveredRangeAtRow(&sensors[i], 2000000);
        if (r.begin != INT_MIN) {
            ranges[nRanges++] = r;
        }
    }

    qsort(ranges, nRanges, sizeof(range), cmp);

    range_stack s = {0};

    for (int i = 0; i < nRanges; ++i) {
        if (range_stack_empty(&s)) {
            range_stack_push(&s, ranges[i]);
        } else {
            range* r = range_stack_peek_ref(&s);
            if (r->begin <= ranges[i].end) {
                r->end = ranges[i].end > r->end ? ranges[i].end : r->end;
            } else {
                range_stack_push(&s, ranges[i]);
            }
        }
    }
    assert(range_stack_size(&s) == 1);
    printf("%d\n", s.elems[0].end - s.elems[0].begin);
}

FUNCTION_DEFN_FOR_YDS(2022, fifteen, b) {
    UNUSED_ARG(fp);
}
