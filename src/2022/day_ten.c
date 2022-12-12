#include "../common.h"

#define SPECIAL_CYCLE_COUNT 6
#define SCREEN_ROWS 6
#define SCREEN_COLS 40

static const int specialCycles[SPECIAL_CYCLE_COUNT] =
    {20, 60, 100, 140, 180, 220};

typedef struct {
    char pixels[SCREEN_ROWS][SCREEN_COLS + 1];
} screen;

typedef struct {
    int cycle;
    int total;
    int x;
    screen screen;
} state;

typedef enum {
    NOOP,
    ADDX,
} cmdType;

typedef struct {
    cmdType type;
    int value;
} cmd;

static void readInput(FILE* fp, void (*handler)(cmd, void*), void* userData) {
    char buffer[32];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (buffer[0] == 'n') {
            handler((cmd){.type = NOOP}, userData);
        } else {
            cmd c = {.type = ADDX};
            assert(sscanf(buffer + 5, "%d", &c.value) == 1);
            handler(c, userData);
        }
    }
}

static void handlerA(cmd c, void* userData) {
    state* s = (state*)userData;
    const int instrLen = c.type == NOOP ? 1 : 2;
    for (int ic = 0; ic < instrLen; ++ic) {
        s->cycle += 1;
        for (int i = 0; i < SPECIAL_CYCLE_COUNT; ++i) {
            if (s->cycle == specialCycles[i]) {
                s->total += (s->cycle * s->x);
                break;
            }
        }
    }
    if (c.type == ADDX) {
        s->x += c.value;
    }
}

static void handlerB(cmd c, void* userData) {
    state* s = (state*)userData;
    const int instrLen = c.type == NOOP ? 1 : 2;
    for (int ic = 0; ic < instrLen; ++ic) {
        s->cycle += 1;
        const int pixelIdx = s->cycle - 1;
        const int screenCol = pixelIdx % SCREEN_COLS;
        const int screenRow = pixelIdx / SCREEN_COLS;
        if (screenCol >= s->x - 1 && screenCol <= s->x + 1) {
            s->screen.pixels[screenRow][screenCol] = '#';
        } else {
            s->screen.pixels[screenRow][screenCol] = '.';
        }
    }
    if (c.type == ADDX) {
        s->x += c.value;
    }
}

FUNCTION_DEFN_FOR_YDS(2022, ten, a) {
    state s = {.x = 1};
    readInput(fp, handlerA, &s);
    printf("%d\n", s.total);
}

FUNCTION_DEFN_FOR_YDS(2022, ten, b) {
    state s = {.x = 1};
    readInput(fp, handlerB, &s);
    for (int i = 0; i < SCREEN_ROWS; ++i) {
        printf("%s\n", s.screen.pixels[i]);
    }
}
