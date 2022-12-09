#include "../common.h"
#include "../utils/coords.h"

#define MAX_SEEN 8192

typedef struct {
    coord* knots;
    coord* seen;
    int knotCount;
    int seenCount;
} state;

static int delta(int a) {
    return a == 0 ? 0 : a > 0 ? 1 : -1;
}

static void updateTail(const coord* head, coord* tail) {
    if (coord_adjacent(head, tail)) {
        return;
    }

    const int dx = delta(head->x - tail->x);
    const int dy = delta(head->y - tail->y);
    tail->x += dx;
    tail->y += dy;
}

static void updateSeen(state* s) {
    const coord tail = s->knots[s->knotCount - 1];
    int low = 0;
    int high = s->seenCount - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = coord_cmp(&tail, &s->seen[mid]);
        if (cmp == 0) {
            // Already here, done.
            return;
        } else if (cmp == 1) {
            // Key is greater.
            low = mid + 1;
        } else {
            // Key is less.
            high = mid - 1;
        }
    }
    // If we get here, the insertion point is high + 1.
    const int ip = high + 1;
    assert(s->seenCount < MAX_SEEN);
    if (ip == s->seenCount) {
        // end of the array.
        s->seen[s->seenCount++] = tail;
    } else {
        // Need to shift over then insert.
        memmove(&s->seen[ip + 1],
                &s->seen[ip],
                sizeof(coord) * (s->seenCount - ip));
        s->seen[ip] = tail;
        s->seenCount += 1;
    }
}

static void readInput(FILE* fp, state* s) {
    UNUSED_ARG(s);
    char buffer[16];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        char d;
        int amt, dx = 0, dy = 0;
        assert(sscanf(buffer, "%c %d", &d, &amt) == 2);
        switch (d) {
        case 'U':
            dy = 1;
            break;
        case 'D':
            dy = -1;
            break;
        case 'L':
            dx = -1;
            break;
        case 'R':
            dx = 1;
            break;
        default:
            assert(false);
        }
        for (int i = 0; i < amt; ++i) {
            s->knots[0].x += dx;
            s->knots[0].y += dy;
            for (int j = 1; j < s->knotCount; ++j) {
                updateTail(&s->knots[j - 1], &s->knots[j]);
            }
            updateSeen(s);
        }
    }
}

FUNCTION_DEFN_FOR_YDS(2022, nine, a) {
    coord seen[MAX_SEEN] = {0};
    coord knots[2] = {0};
    state s = {.knots = knots, .seen = seen, .knotCount = 2, .seenCount = 0};
    readInput(fp, &s);
    printf("%d\n", s.seenCount);
}

FUNCTION_DEFN_FOR_YDS(2022, nine, b) {
    coord seen[MAX_SEEN] = {0};
    coord knots[10] = {0};
    state s = {.knots = knots, .seen = seen, .knotCount = 10, .seenCount = 0};
    readInput(fp, &s);
    printf("%d\n", s.seenCount);
}
