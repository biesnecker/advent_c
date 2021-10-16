#include <ctype.h>

#include "../common.h"
#include "../utils/minmax.h"

typedef struct state {
    int x;
    int y;
    char code[32];
    int idx;
} state;

static const char fancyKeypad[5][5] = {{0, 0, '1', 0, 0},
                                       {0, '2', '3', '4', 0},
                                       {'5', '6', '7', '8', '9'},
                                       {0, 'A', 'B', 'C', 0},
                                       {0, 0, 'D', 0, 0}};

static void readInput(FILE* fp, void (*handler)(char, void*), void* userData) {
    char buffer[1000];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        char* p = buffer;
        while (*p != '\0') {
            char c = *p;
            if (c == '\n') {
                handler(0, userData);
            } else {
                handler(c, userData);
            }
            ++p;
        }
    }
}

static void handleMovement(char c, int* x, int* y) {
    switch (c) {
    case 'L':
        *x -= 1;
        break;
    case 'R':
        *x += 1;
        break;
    case 'D':
        *y += 1;
        break;
    case 'U':
        *y -= 1;
        break;
    default:
        assert(false);
    }
}

static void handlerA(char c, void* userData) {
    state* s = userData;
    if (c == 0) {
        int val = (s->y * 3) + s->x + 1;
        assert(val > 0 && val < 10);
        s->code[s->idx++] = val + '0';
    } else {
        handleMovement(c, &s->x, &s->y);
        s->x = clamp(s->x, 0, 2);
        s->y = clamp(s->y, 0, 2);
    }
}

static void handlerB(char c, void* userData) {
    state* s = userData;
    if (c == 0) {
        char code = fancyKeypad[s->y][s->x];
        assert(code != 0);
        s->code[s->idx++] = code;
    } else {
        int newX = s->x;
        int newY = s->y;
        handleMovement(c, &newX, &newY);
        clamp(newX, 0, 4);
        clamp(newY, 0, 4);
        if (fancyKeypad[newY][newX] != 0) {
            s->x = newX;
            s->y = newY;
        }
    }
}

FUNCTION_DEFN_FOR_YDS(2016, two, a) {
    state s = {0};
    s.x = 1;
    s.y = 1;
    readInput(fp, handlerA, &s);
    printf("%s\n", s.code);
}

FUNCTION_DEFN_FOR_YDS(2016, two, b) {
    state s = {0};
    s.x = 0;
    s.y = 2;
    readInput(fp, handlerB, &s);
    printf("%s\n", s.code);
}
