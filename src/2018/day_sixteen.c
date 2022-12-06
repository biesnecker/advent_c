#include "../common.h"

typedef struct {
    int op;
    int a;
    int b;
    int c;
} instruction;

typedef struct {
    int begin_state[4];
    int end_state[4];
    instruction i;
} sample;

typedef enum { IS_INSTRUCTION, IS_SAMPLE } input_type;

typedef struct {
    input_type type;
    union {
        instruction i;
        sample s;
    };
} input;

typedef bool (*registerHandlerFn)(const instruction*, int*);

typedef struct {
    const char* name;
    registerHandlerFn handler;
} registerHandler;

typedef struct {
    uint16_t handlerPossibilities[16];
    int registers[4];
    bool hasResolved;
} p2state;

static void
readInput(FILE* fp, void (*handler)(const input*, void*), void* userData) {
    int mode = 0;
    int line = 0;
    char buffer[64];
    sample currentSample;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (mode == 0) {
            if (line == 0) {
                if (buffer[0] == '\n') {
                    mode += 1;
                } else {
                    assert(buffer[0] == 'B');
                    memset(&currentSample, 0, sizeof(currentSample));
                    assert(sscanf(buffer,
                                  "Before: [%d, %d, %d, %d]",
                                  &currentSample.begin_state[0],
                                  &currentSample.begin_state[1],
                                  &currentSample.begin_state[2],
                                  &currentSample.begin_state[3]) == 4);
                    line += 1;
                }
            } else if (line == 1) {
                assert(sscanf(buffer,
                              "%d %d %d %d",
                              &currentSample.i.op,
                              &currentSample.i.a,
                              &currentSample.i.b,
                              &currentSample.i.c) == 4);
                line += 1;
            } else if (line == 2) {
                assert(buffer[0] == 'A');
                assert(sscanf(buffer,
                              "After:  [%d, %d, %d, %d]",
                              &currentSample.end_state[0],
                              &currentSample.end_state[1],
                              &currentSample.end_state[2],
                              &currentSample.end_state[3]) == 4);
                line += 1;
            } else {
                assert(buffer[0] == '\n');
                input in = {.type = IS_SAMPLE, .s = currentSample};
                handler(&in, userData);
                line = 0;
            }
        } else if (mode == 1) {
            // Whitespace between samples and instructions.
            assert(buffer[0] == '\n');
            mode += 1;
        } else {
            input in = {.type = IS_INSTRUCTION, .i = {0}};
            assert(sscanf(buffer,
                          "%d %d %d %d",
                          &in.i.op,
                          &in.i.a,
                          &in.i.b,
                          &in.i.c) == 4);
            handler(&in, userData);
        }
    }
}

#define CHECK_POSSIBLE_REGISTER(v)                                             \
    do {                                                                       \
        if ((v) < 0 || (v) > 3) {                                              \
            return false;                                                      \
        }                                                                      \
    } while (false)

static bool addr(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->a);
    CHECK_POSSIBLE_REGISTER(i->b);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = reg[i->a] + reg[i->b];
    return true;
}

static bool addi(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->a);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = reg[i->a] + i->b;
    return true;
}

static bool mulr(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->a);
    CHECK_POSSIBLE_REGISTER(i->b);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = reg[i->a] * reg[i->b];
    return true;
}

static bool muli(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->a);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = reg[i->a] * i->b;
    return true;
}

static bool banr(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->a);
    CHECK_POSSIBLE_REGISTER(i->b);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = reg[i->a] & reg[i->b];
    return true;
}

static bool bani(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->a);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = reg[i->a] & i->b;
    return true;
}

static bool borr(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->a);
    CHECK_POSSIBLE_REGISTER(i->b);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = reg[i->a] | reg[i->b];
    return true;
}

static bool bori(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->a);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = reg[i->a] | i->b;
    return true;
}

static bool setr(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->a);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = reg[i->a];
    return true;
}

static bool seti(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = i->a;
    return true;
}

static bool gtir(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->b);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = i->a > reg[i->b] ? 1 : 0;
    return true;
}

static bool gtri(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->a);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = reg[i->a] > i->b ? 1 : 0;
    return true;
}

static bool gtrr(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->a);
    CHECK_POSSIBLE_REGISTER(i->b);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = reg[i->a] > reg[i->b] ? 1 : 0;
    return true;
}

static bool eqir(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->b);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = i->a == reg[i->b] ? 1 : 0;
    return true;
}

static bool eqri(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->a);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = reg[i->a] == i->b ? 1 : 0;
    return true;
}

static bool eqrr(const instruction* i, int* reg) {
    CHECK_POSSIBLE_REGISTER(i->a);
    CHECK_POSSIBLE_REGISTER(i->b);
    CHECK_POSSIBLE_REGISTER(i->c);
    reg[i->c] = reg[i->a] == reg[i->b] ? 1 : 0;
    return true;
}

static const registerHandler registerHandlers[16] = {
    {.name = "addr", .handler = &addr},
    {.name = "addi", .handler = &addi},
    {.name = "mulr", .handler = &mulr},
    {.name = "muli", .handler = &muli},
    {.name = "banr", .handler = &banr},
    {.name = "bani", .handler = &bani},
    {.name = "borr", .handler = &borr},
    {.name = "bori", .handler = &bori},
    {.name = "setr", .handler = &setr},
    {.name = "seti", .handler = &seti},
    {.name = "gtir", .handler = &gtir},
    {.name = "gtri", .handler = &gtri},
    {.name = "gtrr", .handler = &gtrr},
    {.name = "eqir", .handler = &eqir},
    {.name = "eqri", .handler = &eqri},
    {.name = "eqrr", .handler = &eqrr}};

static bool testHandlerOnSample(const sample* s, int handlerIdx) {
    int regState[4] = {0};
    memcpy(regState, &s->begin_state, sizeof(int) * 4);
    if (!registerHandlers[handlerIdx].handler(&s->i, regState)) {
        return false;
    }
    return memcmp(regState, &s->end_state, sizeof(int) * 4) == 0;
}

static void handlerA(const input* in, void* userData) {
    int* threeOrMore = (int*)userData;

    if (in->type == IS_SAMPLE) {
        int possible = 0;
        for (int i = 0; i < 16; i++) {
            if (testHandlerOnSample(&in->s, i)) {
                possible += 1;
            }
        }
        if (possible >= 3) {
            *threeOrMore += 1;
        }
    }
}

static void resolveHandlerPossiblities(const sample* s, p2state* state) {
    if (__builtin_popcount(state->handlerPossibilities[s->i.op]) == 1) {
        return;
    }
    for (int j = 0; j < 16; j++) {
        uint16_t mask = (uint16_t)(1U << j);
        if ((state->handlerPossibilities[s->i.op] & mask) != 0) {
            if (!testHandlerOnSample(s, j)) {
                state->handlerPossibilities[s->i.op] &= ~mask;
            }
        }
    }
    assert(state->handlerPossibilities[s->i.op] != 0);
}

static void finalResolve(p2state* state) {
    bool finished = false;
    while (!finished) {
        finished = true;
        for (int i = 0; i < 16; i++) {
            if (__builtin_popcount(state->handlerPossibilities[i]) > 1) {
                continue;
            }
            uint16_t mask = ~state->handlerPossibilities[i];
            for (int j = 0; j < 16; j++) {
                if (i == j) {
                    continue;
                }
                uint16_t oldState = state->handlerPossibilities[j];
                state->handlerPossibilities[j] &= mask;
                finished =
                    finished && (oldState == state->handlerPossibilities[j]);
            }
        }
    }
    state->hasResolved = true;
}

static void handlerB(const input* in, void* userData) {
    p2state* state = (p2state*)userData;
    if (in->type == IS_SAMPLE) {
        resolveHandlerPossiblities(&in->s, state);
    } else {
        if (!state->hasResolved) {
            finalResolve(state);
        }
        registerHandlerFn r =
            registerHandlers[__builtin_ctz(
                                 state->handlerPossibilities[in->i.op])]
                .handler;
        assert(r(&in->i, state->registers));
    }
}

static void handlerDebug(const input* in, void* userData) {
    p2state* state = (p2state*)userData;
    if (in->type == IS_SAMPLE) {
        resolveHandlerPossiblities(&in->s, state);
    }
}

FUNCTION_DEFN_FOR_YDS(2018, sixteen, a) {
    int res = 0;
    readInput(fp, handlerA, &res);
    printf("%d\n", res);
}

FUNCTION_DEFN_FOR_YDS(2018, sixteen, b) {
    p2state s = {0};
    for (int i = 0; i < 16; i++) {
        s.handlerPossibilities[i] = UINT16_MAX;
    }
    readInput(fp, handlerB, &s);
    printf("%d\n", s.registers[0]);
}

FUNCTION_DEFN_FOR_YDS(2018, sixteen, debug) {
    p2state s = {0};
    for (int i = 0; i < 16; i++) {
        s.handlerPossibilities[i] = UINT16_MAX;
    }
    readInput(fp, handlerDebug, &s);
    finalResolve(&s);
    for (int i = 0; i < 16; i++) {
        assert(__builtin_popcount(s.handlerPossibilities[i]) == 1);
        int rhi = __builtin_ctz(s.handlerPossibilities[i]);
        printf("%d: %s\n", i, registerHandlers[rhi].name);
    }
}
