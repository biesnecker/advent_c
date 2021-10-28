#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"
#include "../utils/hashset.h"
#include "../utils/heap.h"
#include "../utils/minmax.h"
#include "../utils/strings.h"
#include "../utils/vector.h"
#include "../utils/xxhash.h"

static const uint64_t elevatorMaskLo = 0x80ULL;   // 8th bit
static const uint64_t elevatorMaskHi = 0x8000ULL; // 16th bit
static const uint64_t elevatorMaskZero = 0xFFFFFFFFFFFF7F7FULL;

#define MICROCHIPS_FOR_FLOOR(f) (((f)*2) + 1)
#define GENERATORS_FOR_FLOOR(f) ((f)*2)

typedef union {
    uint64_t data;
    uint16_t floors[4];
    uint8_t chipGens[8];
} state;

static const state zeroState = {.data = 0};

static int compareState(const void* a, const void* b) {
    const state* s = a;
    const state* t = b;
    if (s->data < t->data) {
        return -1;
    } else if (s->data > t->data) {
        return 1;
    }
    return 0;
}

static uint64_t hashState(const void* a) {
    return xxhashFromData(a, sizeof(state));
}

typedef struct position {
    state s;
    uint32_t steps;
} position;

static int comparePosition(const void* a, const void* b) {
    const position* p = a;
    const position* q = b;
    if (p->steps < q->steps) {
        return -1;
    } else if (p->steps > q->steps) {
        return 1;
    }
    return 0;
}

typedef struct inputState {
    vector names;
    state starting;
    int floorIdx;
} inputState;

// Elevator position is encoded in the top bits of the first two bytes.
static uint8_t getElevatorPos(state state) {
    return (state.data & elevatorMaskLo) >> 7 |
           (state.data & elevatorMaskHi) >> 14;
}

// Overwrites existing elevator position information in the state.
static state setElevatorPos(state state, uint8_t epos) {
    state.data |= (uint64_t)(epos & 0x1) << 7 | (uint64_t)(epos & 0x2) << 14;
    return state;
}

static void
readInput(FILE* fp, void (*handler)(const char* s, void*), void* userData) {
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        stripNewline(buffer);
        handler(buffer, userData);
    }
}

static size_t indexForName(vector* v, const char* name) {
    size_t idx = 0;
    VECTOR_FOREACH_TYPED(v, const char*, n) {
        if (strcmp(*n, name) == 0) {
            return idx;
        }
        ++idx;
    }
    size_t ret = v->len;
    char* dup = strdup(name);
    vectorPush(v, &dup);
    return ret;
}

static void parseLine(char* line, inputState* s) {
    if (strstr(line, "nothing") != NULL) {
        return;
    }

    char* currentToken = strtok(line, " ");
    char* previousToken = NULL;
    while (currentToken != NULL) {
        if (previousToken != NULL && strcmp(currentToken, "generator") == 0) {
            size_t idx = indexForName(&s->names, previousToken);
            s->starting.chipGens[s->floorIdx * 2] |= (1 << idx);
        } else if (previousToken != NULL &&
                   strcmp(currentToken, "compatible") == 0) {
            size_t idx = indexForName(&s->names, previousToken);
            s->starting.chipGens[s->floorIdx * 2 + 1] |= (1 << idx);
        }
        previousToken = currentToken;
        currentToken = strtok(NULL, " ");
    }
}

static void removeNonAlpha(char* s) {
    while (*s != '\0') {
        if (!isalpha(*s)) {
            *s = ' ';
        }
        ++s;
    }
}

static void handlerA(const char* line, void* userData) {
    inputState* s = userData;

    char* mutableLine = strdup(line);
    removeNonAlpha(mutableLine);
    parseLine(mutableLine, s);
    free(mutableLine);

    s->floorIdx += 1;
}

static state _calculateNewState(state s,
                                uint8_t elevatorPos,
                                uint8_t newElevatorPos,
                                int i,
                                int j,
                                int type1,
                                int type2) {
    // Remove the elevator data.
    s.data &= elevatorMaskZero;

    uint16_t itemOneMask = (1U << i) << (type1 * 8);
    uint16_t itemTwoMask = j == -1 ? 0 : (1U << j) << (type2 * 8);

    uint16_t* oldFloor = &s.floors[elevatorPos];
    uint16_t* newFloor = &s.floors[newElevatorPos];

    // Items don't exist on the floor.
    if (((*oldFloor & itemOneMask) == 0) ||
        (j != -1 && ((*oldFloor & itemTwoMask) == 0))) {
        return zeroState;
    }

    (*oldFloor) &= (uint16_t)(~(itemOneMask | itemTwoMask));
    (*newFloor) |= (uint16_t)(itemOneMask | itemTwoMask);

    uint16_t oldFloorUnprotected =
        s.chipGens[MICROCHIPS_FOR_FLOOR(elevatorPos)] &
        ~(s.chipGens[GENERATORS_FOR_FLOOR(elevatorPos)]);
    if (oldFloorUnprotected != 0 &&
        s.chipGens[GENERATORS_FOR_FLOOR(elevatorPos)] != 0) {
        return zeroState;
    }
    uint16_t newFloorUnprotected =
        s.chipGens[MICROCHIPS_FOR_FLOOR(newElevatorPos)] &
        ~(s.chipGens[GENERATORS_FOR_FLOOR(newElevatorPos)]);
    if (newFloorUnprotected != 0 &&
        s.chipGens[GENERATORS_FOR_FLOOR(newElevatorPos)]) {
        return zeroState;
    }
    return setElevatorPos(s, newElevatorPos);
}

static void runSimulation(state starting, int itemTypes) {
    size_t iters = 0;
    size_t shortest = SIZE_MAX;

    heap pqueue;
    heapInit(&pqueue, 1U << 10, sizeof(position), comparePosition, NULL);

    hashset seen;
    hashsetInit(&seen, 1U << 10, sizeof(state), compareState, hashState, NULL);

    position startingPosition = {.s = starting, .steps = 0};

    heapPush(&pqueue, &startingPosition);

    position* p;
    while ((p = heapPeek(&pqueue)) != NULL) {
        ++iters;
        state oldState = p->s;
        size_t steps = p->steps;
        heapPop(&pqueue);

        size_t newSteps = steps + 1;

        if (newSteps >= shortest) {
            // Already longer than the longest path, so bail.
            continue;
        }

        bool isNew;
        hashsetLookupOrInsert(&seen, &oldState, &isNew);
        if (!isNew) {
            // Already seen this one.
            continue;
        }

        uint8_t elevatorPos = getElevatorPos(oldState);
        uint8_t elevatorLow = elevatorPos > 0 ? elevatorPos - 1 : 0;
        uint8_t elevatorHigh = elevatorPos < 3 ? elevatorPos + 1 : 3;

        bool firstFloorEmpty = (oldState.floors[0] & 0x7f7f) == 0;
        bool secondFloorEmpty = (oldState.floors[1] & 0x7f7f) == 0;

        if (elevatorLow == 0 && firstFloorEmpty) {
            elevatorLow = 1;
        } else if (elevatorLow == 1 && firstFloorEmpty && secondFloorEmpty) {
            elevatorLow = 2;
        }

        for (uint8_t newElevatorPos = elevatorLow;
             newElevatorPos <= elevatorHigh;
             ++newElevatorPos) {
            if (newElevatorPos == elevatorPos) {
                continue;
            }

            for (int j = -1; j < itemTypes - 1; ++j) {
                for (int i = 0; i < itemTypes; ++i) {
                    for (int type1 = 0; type1 < 2; ++type1) {
                        for (int type2 = 0; type2 < 2; ++type2) {
                            if (j == -1 && type2 == 1) {
                                continue;
                            }
                            if (j != -1 && ((type1 != type2 && i != j) ||
                                            (type1 == type2 && i == j))) {
                                // One is a generator, the other is a
                                // microchip, and they're not the same, or
                                // they're both pointing to the same thing.
                                continue;
                            }

                            state newState = _calculateNewState(oldState,
                                                                elevatorPos,
                                                                newElevatorPos,
                                                                i,
                                                                j,
                                                                type1,
                                                                type2);
                            if (newState.data == 0) {
                                // Not a valid state.
                                continue;
                            }

                            if (((newState.data & elevatorMaskZero) &
                                 0xFFFFFFFFFFFFULL) == 0) {
                                // Everything is on the fourth floor.
                                if (newSteps < shortest) {
                                    shortest = newSteps;
                                }
                                continue;
                            }

                            position newPosition = {.s = newState,
                                                    .steps = newSteps};

                            heapPush(&pqueue, &newPosition);
                        }
                    }
                }
            }
        }
    }

    printf("Found shortest = %zu after iters = %zu\n", shortest, iters);

    hashsetFinalize(&seen);
    heapFinalize(&pqueue);
}

FUNCTION_DEFN_FOR_YDS(2016, eleven, a) {
    inputState is = {0};
    vectorInit(&is.names, 10, sizeof(char*), free);
    readInput(fp, handlerA, &is);

    runSimulation(is.starting, (int)vectorSize(&is.names));
    vectorFinalize(&is.names);
}

FUNCTION_DEFN_FOR_YDS(2016, eleven, b) {
    inputState is = {0};
    vectorInit(&is.names, 10, sizeof(char*), free);
    readInput(fp, handlerA, &is);
    is.starting.data |= 0x6060;

    runSimulation(is.starting, (int)vectorSize(&is.names) + 2);
    vectorFinalize(&is.names);
}
