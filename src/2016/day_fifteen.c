#include "../common.h"
#include "../utils/vector.h"

typedef struct disc {
    int id;
    int positions;
    int starting;
} disc;

static void readInput(FILE* fp,
                      void (*handler)(const disc* d, void* userData),
                      void* userData) {
    int id, positions, starting;
    while (
        fscanf(fp,
               "Disc #%d has %d positions; at time=0, it is at position %d.\n",
               &id,
               &positions,
               &starting) != EOF) {
        disc d = {.id = id, .starting = starting, .positions = positions};
        handler(&d, userData);
    }
}

static void inputHandler(const disc* d, void* userData) {
    vector* v = userData;
    vectorPush(v, d);
}

static int findSolution(vector* v) {
    disc* firstDisc = vectorAt(v, 0);
    int startTime =
        firstDisc->positions -
        ((firstDisc->id + firstDisc->starting) % firstDisc->positions);
    int step = firstDisc->positions;
    bool found;
    do {
        found = true;
        VECTOR_FOREACH_TYPED(v, disc, d) {
            if (((startTime + d->id + d->starting) % d->positions) != 0) {
                found = false;
                startTime += step;
                break;
            }
        }
    } while (!found);
    return startTime;
}

FUNCTION_DEFN_FOR_YDS(2016, fifteen, a) {
    vector v;
    vectorInit(&v, 8, sizeof(disc));
    readInput(fp, inputHandler, &v);
    printf("%d\n", findSolution(&v));
    vectorFinalize(&v);
}

FUNCTION_DEFN_FOR_YDS(2016, fifteen, b) {
    vector v;
    vectorInit(&v, 8, sizeof(disc));
    readInput(fp, inputHandler, &v);
    int id = vectorSize(&v) + 1;
    vectorPush(&v, &(disc){.starting = 0, .positions = 11, .id = id});
    printf("%d\n", findSolution(&v));
    vectorFinalize(&v);
}
