#include <inttypes.h>

#include "../common.h"

typedef struct {
    int64_t depth;
    int64_t pos;
    int64_t aim;
} submarine;

typedef enum { FORWARD, UP, DOWN } command;

static void
readInput(FILE* fp, void (*handler)(command, int, submarine*), submarine* sub) {
    char buffer[512];
    char cmdBuffer[50];
    int x;
    command cmd;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        assert(sscanf(buffer, "%s %d", cmdBuffer, &x) == 2);
        if (strcmp(cmdBuffer, "forward") == 0) {
            cmd = FORWARD;
        } else if (strcmp(cmdBuffer, "up") == 0) {
            cmd = UP;
        } else if (strcmp(cmdBuffer, "down") == 0) {
            cmd = DOWN;
        } else {
            assert(false);
        }
        handler(cmd, x, sub);
    }
}

static void handlerA(command cmd, int x, submarine* sub) {
    switch (cmd) {
    case FORWARD:
        sub->pos += x;
        break;
    case UP:
        sub->depth -= x;
        if (sub->depth < 0) {
            sub->depth = 0;
        }
        break;
    case DOWN:
        sub->depth += x;
        break;
    }
}

static void handlerB(command cmd, int x, submarine* sub) {
    switch (cmd) {
    case FORWARD:
        sub->pos += x;
        sub->depth += (x * sub->aim);
        if (sub->depth < 0) {
            sub->depth = 0;
        }
        break;
    case UP:
        sub->aim -= x;
        break;
    case DOWN:
        sub->aim += x;
        break;
    }
}

FUNCTION_DEFN_FOR_YDS(2021, two, a) {
    submarine sub = {0};
    readInput(fp, handlerA, &sub);
    printf("%" PRId64 "\n", sub.depth * sub.pos);
}

FUNCTION_DEFN_FOR_YDS(2021, two, b) {
    submarine sub = {0};
    readInput(fp, handlerB, &sub);
    printf("%" PRId64 "\n", sub.depth * sub.pos);
}
