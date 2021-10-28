#include <limits.h>
#include <stdint.h>
#include <unistd.h>

#include "../common.h"
#include "../utils/heap.h"
#include "../utils/md5.h"

typedef struct doorState {
    bool doors[4];
} doorState;

typedef struct location {
    int x, y;
} location;

typedef struct state {
    location loc;
    char* path;
    size_t pathLen;
    size_t pathCap;
} state;

static void initState(state* s, location loc) {
    s->loc = loc;
    s->path = calloc(16, sizeof(char));
    assert(s->path != NULL);
    s->pathLen = 0;
    s->pathCap = 16;
}

static inline bool isOpenChar(char c) {
    return (c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f');
}

static void
doorstateForState(state* s, char* salt, size_t saltLen, doorState* ds) {
    char md5buffer[33] = {0};
    char dataBuffer[s->pathLen + saltLen];
    char* buffer = dataBuffer;
    memcpy(buffer, salt, saltLen);
    memcpy(buffer + saltLen, s->path, s->pathLen);
    md5HexToBuffer(buffer, s->pathLen + saltLen, md5buffer);

    for (int i = 0; i < 4; ++i) {
        ds->doors[i] = isOpenChar(md5buffer[i]);
    }
}

static void appendCharToState(state* dest, char c) {
    if (dest->pathLen + 1 >= dest->pathCap) {
        size_t newCap = dest->pathCap + 16;
        dest->path = realloc(dest->path, newCap);
        dest->pathCap = newCap;
        assert(dest->path != NULL);
    }
    dest->path[dest->pathLen++] = c;
}

static void copyState(state* dest, const state* source) {
    dest->loc = source->loc;
    dest->pathLen = source->pathLen;
    dest->pathCap = source->pathCap;
    dest->path = calloc(source->pathCap, sizeof(char));
    assert(dest->path != NULL);
    memcpy(dest->path, source->path, source->pathCap);
    for (size_t i = 0; i < source->pathCap; ++i) {
        printf("[%d %d] ", source->path[i], dest->path[i]);
    }
    puts("");
}

static int compareState(const void* a, const void* b) {
    return ((state*)a)->pathLen - ((state*)b)->pathLen;
}

static void freeState(void* a) {
    free(((state*)a)->path);
}

static char* readInput(FILE* fp) {
    char buffer[50];
    assert(fscanf(fp, "%s\n", buffer) == 1);
    return strdup(buffer);
}

static char directions[] = {'U', 'D', 'L', 'R'};

static char* findPath(location goal, char* salt, size_t saltLen) {
    UNUSED_ARG(goal);
    state initialState;
    initState(&initialState, (location){.x = 0, .y = 0});

    size_t shortest = SIZE_MAX;
    char* shortestPath = NULL;

    heap pqueue;
    heapInit(&pqueue, 1 << 10, sizeof(state), compareState, freeState);

    heapPush(&pqueue, &initialState);

    state* st;
    while ((st = heapPeek(&pqueue)) != NULL) {
        printf("(%d, %d) [%zu, %zu] %s\n",
               st->loc.x,
               st->loc.y,
               st->pathLen,
               st->pathCap,
               st->path);
        state s;
        copyState(&s, st);
        heapPop(&pqueue);

        printf("(%d, %d) [%zu, %zu] %s\n",
               s.loc.x,
               s.loc.y,
               s.pathLen,
               s.pathCap,
               s.path);

        if (s.pathLen + 1 >= shortest) {
            // Don't pursue paths longer than an already found solution.
            continue;
        }

        doorState ds = {0};
        doorstateForState(&s, salt, saltLen, &ds);

        for (int i = 0; i < 4; ++i) {
            if (!ds.doors[i]) {
                // Door is closed.
                continue;
            }
            char door = directions[i];
            printf("%c is open.\n", door);

            if ((door == 'U' && s.loc.y == 0) ||
                (door == 'D' && s.loc.y == 3) ||
                (door == 'L' && s.loc.x == 0) ||
                (door == 'R' && s.loc.x == 3)) {
                // Don't go off the map.
                continue;
            }

            state newState;
            copyState(&newState, &s);
            switch (door) {
            case 'U':
                newState.loc.y -= 1;
                break;
            case 'D':
                newState.loc.y += 1;
                break;
            case 'L':
                newState.loc.x -= 1;
                break;
            case 'R':
                newState.loc.x += 1;
                break;
            default:
                assert(false);
            }
            appendCharToState(&newState, door);
            printf("Old state: %.*s ", (int)s.pathLen, s.path);
            printf("New state: %.*s ", (int)newState.pathLen, newState.path);
            printf("ox: %d oy: %d ", s.loc.x, s.loc.y);
            printf("nx: %d ny: %d\n", newState.loc.x, newState.loc.y);

            if (newState.loc.x == goal.x && newState.loc.y == goal.y) {
                if (newState.pathLen < shortest) {
                    shortest = newState.pathLen;
                    free(shortestPath);
                    shortestPath = calloc(newState.pathLen + 1, sizeof(char));
                    memcpy(shortestPath, newState.path, newState.pathLen);
                }
                freeState(&newState);
                continue;
            }

            heapPush(&pqueue, &newState);
        }
    }
    return shortestPath;
}

FUNCTION_DEFN_FOR_YDS(2016, seventeen, a) {
    sleep(2);
    char* salt = readInput(fp);

    char* path = findPath((location){.x = 3, .y = 3}, salt, strlen(salt));

    printf("%s\n", path);

    free(path);
    free(salt);
}

FUNCTION_DEFN_FOR_YDS(2016, seventeen, b) {
    UNUSED_ARG(fp);
}
