#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../macros.h"
#include "../utils/bitset.h"

typedef struct coords {
    int x;
    int y;
} coords;

typedef struct locationState {
    int direction;
    coords c;
} locationState;

typedef struct locationHistoryState {
    locationState loc;
    bitset history;
} locationHistoryState;

static void readInput(FILE* fp,
                      bool (*handler)(char d, uint32_t n, void* userData),
                      void* userData) {
    char buffer[1024];
    assert(fgets(buffer, sizeof(buffer), fp) != NULL);

    char* pos = buffer;
    while (pos != NULL) {
        char d;
        uint32_t n;
        assert(sscanf(pos, "%c%u", &d, &n) == 2);
        if (!handler(d, n, userData)) {
            break;
        }
        char* nextPos = strchr(pos, ',');
        if (nextPos != NULL) {
            nextPos += 2;
        }
        pos = nextPos;
    }
}

static void updateLocationState(char d, uint32_t n, locationState* s) {
    if (d == 'R') {
        ++s->direction;
    } else {
        --s->direction;
        if (s->direction < 0) {
            s->direction = 3;
        }
    }
    s->direction %= 4;
    switch (s->direction) {
    case 0:
        s->c.y += n;
        break;
    case 1:
        s->c.x += n;
        break;
    case 2:
        s->c.y -= n;
        break;
    case 3:
        s->c.x -= n;
        break;
    default:
        assert(false);
    }
}

static int distance(coords* c) {
    return abs(c->x) + abs(c->y);
}

static bool handlerA(char d, uint32_t n, void* userData) {
    updateLocationState(d, n, (locationState*)userData);
    return true;
}

static bool handlerB(char d, uint32_t n, void* userData) {
    locationHistoryState* s = userData;
    coords startingLoc = s->loc.c;
    updateLocationState(d, n, &s->loc);

    coords currentLoc;

    int dir, start, end, *movingAxisTarget;

    // Update where we've been. Do a bunch of setup so that the iteration code
    // is not repeated, though maybe the setup code ends up longer than the
    // iteration code ::sigh::
    if (startingLoc.x != s->loc.c.x) {
        // we moved along the x axis.
        dir = startingLoc.x < s->loc.c.x ? 1 : -1;
        start = startingLoc.x + dir;
        end = s->loc.c.x + dir;
        movingAxisTarget = &currentLoc.x;
        currentLoc.y = startingLoc.y;
    } else {
        // we moved along the y axis.
        dir = startingLoc.y < s->loc.c.y ? 1 : -1;
        start = startingLoc.y + dir;
        end = s->loc.c.y + dir;
        movingAxisTarget = &currentLoc.y;
        currentLoc.x = startingLoc.x;
    }
    for (int i = start; i != end; i += dir) {
        *movingAxisTarget = i;

        // Now currentLoc has the coordinate of a place we've been.
        int x = currentLoc.x + 255;
        int y = currentLoc.y + 255;
        assert(x >= 0 && x < 512);
        assert(y >= 0 && y < 512);
        int idx = x + (y << 9);

        if (bitsetCheck(&s->history, idx)) {
            printf("%d\n", distance(&currentLoc));
            return false;
        } else {
            bitsetSet(&s->history, idx, true);
        }
    }
    return true;
}

FUNCTION_DEFN_FOR_YDS(2016, one, a) {
    locationState s = {0};
    readInput(fp, handlerA, &s);
    printf("%d\n", distance(&s.c));
}

FUNCTION_DEFN_FOR_YDS(2016, one, b) {
    locationHistoryState s = {0};
    bitsetInit(&s.history, 512 * 512);
    readInput(fp, handlerB, &s);
    bitsetFinalize(&s.history);
}
