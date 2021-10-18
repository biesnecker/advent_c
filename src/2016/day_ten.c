#include <stdio.h>
#include <stdlib.h>

#include "../common.h"
#include "../utils/list.h"

#define MAXBOTS 256
#define MAXOUTPUTS 32
#define MAXINPUTS 32

typedef enum targetType {
    UNSET,
    OUTPUT,
    BOT,
} targetType;

typedef struct bot {
    uint16_t hi;
    uint16_t lo;
    targetType hiTarget;
    targetType loTarget;
    union {
        struct {
            struct bot* destination;
        } hiToBot;
        struct {
            uint16_t* destination;
        } hiToOutput;
    };
    union {
        struct {
            struct bot* destination;
        } loToBot;
        struct {
            uint16_t* destination;
        } loToOutput;
    };
} bot;

typedef struct input {
    uint16_t value;
    bot* destination;
} input;

typedef struct output {
    uint16_t value;
} output;

typedef enum op {
    SET_INPUT,
    HIGH_TO_BOT,
    LOW_TO_BOT,
    HIGH_TO_OUTPUT,
    LOW_TO_OUTPUT,
} op;

typedef struct instruction {
    op opcode;
    union {
        struct {
            uint16_t value;
            uint16_t destination;
        } setInput;
        struct {
            uint16_t source;
            uint16_t destination;
        } hilo;
    };
} instruction;

typedef struct state {
    input inputs[MAXINPUTS];
    int inputIdx;
    bot bots[MAXBOTS];
    uint16_t outputs[MAXOUTPUTS];
} state;

static void
readInput(FILE* fp, void (*handler)(instruction*, void*), void* userData) {
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        instruction i = {0};
        if (buffer[0] == 'v') {
            uint16_t value, dest;
            assert(sscanf(buffer, "value %hd goes to bot %hd", &value, &dest) ==
                   2);
            i.opcode = SET_INPUT;
            i.setInput.value = value;
            i.setInput.destination = dest;
            handler(&i, userData);
        } else if (buffer[0] == 'b') {
            uint16_t source, low, high;
            char lowTarget[8], highTarget[8];
            assert(sscanf(buffer,
                          "bot %hd gives low to %s %hd and high to %s %hd",
                          &source,
                          lowTarget,
                          &low,
                          highTarget,
                          &high) == 5);

            if (lowTarget[0] == 'o') {
                i.opcode = LOW_TO_OUTPUT;
            } else {
                i.opcode = LOW_TO_BOT;
            }
            i.hilo.source = source;
            i.hilo.destination = low;
            handler(&i, userData);

            if (highTarget[0] == 'o') {
                i.opcode = HIGH_TO_OUTPUT;
            } else {
                i.opcode = HIGH_TO_BOT;
            }
            i.hilo.source = source;
            i.hilo.destination = high;
            handler(&i, userData);
        } else {
            assert(false);
        }
    }
}

static bool assignValueToBot(bot* b, uint16_t value) {
    if (b->lo == 0) {
        // Assign to low by default.
        b->lo = value;
        return false;
    } else if (b->lo > value) {
        b->hi = b->lo;
        b->lo = value;
        return true;
    } else {
        b->hi = value;
        return true;
    }
}

static void handleInstruction(state* s, instruction* in) {
    switch (in->opcode) {
    case SET_INPUT: {
        bot* b = &s->bots[in->setInput.destination];
        s->inputs[s->inputIdx++] =
            (input){.destination = b, .value = in->setInput.value};
        break;
    }
    case HIGH_TO_OUTPUT: {
        bot* b = &s->bots[in->hilo.source];
        uint16_t* output = &s->outputs[in->hilo.destination];
        b->hiToOutput.destination = output;
        b->hiTarget = OUTPUT;
        break;
    }
    case LOW_TO_OUTPUT: {
        bot* b = &s->bots[in->hilo.source];
        uint16_t* output = &s->outputs[in->hilo.destination];
        b->loToOutput.destination = output;
        b->loTarget = OUTPUT;
        break;
    }
    case HIGH_TO_BOT: {
        bot* source = &s->bots[in->hilo.source];
        bot* destination = &s->bots[in->hilo.destination];
        source->hiToBot.destination = destination;
        source->hiTarget = BOT;
        break;
    }
    case LOW_TO_BOT: {
        bot* source = &s->bots[in->hilo.source];
        bot* destination = &s->bots[in->hilo.destination];
        source->loToBot.destination = destination;
        source->loTarget = BOT;
        break;
    }
    default:
        assert(false);
    }
}

static void propogateFromBot(bot* b) {
    if (b->hi == 0 || b->lo == 0) {
        return;
    }

    if (b->hiTarget == UNSET || b->loTarget == UNSET) {
        return;
    }

    switch (b->hiTarget) {
    case BOT:
        assignValueToBot(b->hiToBot.destination, b->hi);
        propogateFromBot(b->hiToBot.destination);
        break;
    case OUTPUT:
        *b->hiToOutput.destination = b->hi;
        break;
    default:
        assert(false);
    }

    switch (b->loTarget) {
    case BOT:
        assignValueToBot(b->loToBot.destination, b->lo);
        propogateFromBot(b->loToBot.destination);
        break;
    case OUTPUT:
        *b->loToOutput.destination = b->lo;
        break;
    default:
        assert(false);
    }
}

static void runSimulation(state* s) {
    for (int i = 0; i < s->inputIdx; ++i) {
        assert(s->inputs[i].destination != NULL);
        assignValueToBot(s->inputs[i].destination, s->inputs[i].value);
        propogateFromBot(s->inputs[i].destination);
    }
}

static void handler(instruction* in, void* userData) {
    state* s = userData;
    handleInstruction(s, in);
}

FUNCTION_DEFN_FOR_YDS(2016, ten, a) {
    state s = {0};
    readInput(fp, handler, &s);
    runSimulation(&s);

    for (int i = 0; i < MAXBOTS; ++i) {
        if (s.bots[i].hi == 61 && s.bots[i].lo == 17) {
            printf("%d\n", i);
            break;
        }
    }
}

FUNCTION_DEFN_FOR_YDS(2016, ten, b) {
    state s = {0};
    readInput(fp, handler, &s);
    runSimulation(&s);

    uint16_t product = 1;
    for (int i = 0; i < 3; ++i) {
        product *= s.outputs[i];
    }
    printf("%hd\n", product);
}
