#include <stdio.h>

#include "../common.h"

typedef enum op {
    RECT,
    ROTX,
    ROTY,
} op;

typedef struct instr {
    op opcode;
    int a;
    int b;
} instr;

#define COLS 6
#define ROWS 50

typedef bool screen[COLS][ROWS];
typedef screen* screenptr;

static void parseRect(const char* s, instr* i) {
    i->opcode = RECT;
    assert(sscanf(s, "rect %dx%d\n", &i->a, &i->b) == 2);
}

static void parseRotate(const char* s, instr* i) {
    if (strchr(s, 'x') != NULL) {
        i->opcode = ROTX;
        assert(sscanf(s, "rotate column x=%d by %d", &i->a, &i->b) == 2);
    } else {
        i->opcode = ROTY;
        assert(sscanf(s, "rotate row y=%d by %d", &i->a, &i->b) == 2);
    }
}

static void
readInput(FILE* fp, void (*handler)(const instr*, void*), void* userData) {
    char buffer[32];

    instr i = {0};

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        switch (buffer[1]) {
        case 'e':
            // rect
            parseRect(buffer, &i);
            break;
        case 'o':
            // rotate
            parseRotate(buffer, &i);
            break;
        default:
            assert(false);
        }
        handler(&i, userData);
    }
}

static void handlerA(const instr* instruction, void* userData) {
    screenptr s = userData;
    if (instruction->opcode == RECT) {
        for (int i = 0; i < instruction->b; ++i) {
            for (int j = 0; j < instruction->a; ++j) {
                (*s)[i][j] = true;
            }
        }
    } else if (instruction->opcode == ROTX) {
        bool temp[COLS] = {false};
        for (int i = 0; i < COLS; ++i) {
            if ((*s)[i][instruction->a]) {
                int newOffset = (i + instruction->b) % COLS;
                temp[newOffset] = true;
            }
        }
        for (int i = 0; i < COLS; ++i) {
            (*s)[i][instruction->a] = temp[i];
        }
    } else if (instruction->opcode == ROTY) {
        bool temp[ROWS] = {false};
        for (int i = 0; i < ROWS; ++i) {
            if ((*s)[instruction->a][i]) {
                int newOffset = (i + instruction->b) % ROWS;
                temp[newOffset] = true;
            }
        }
        for (int i = 0; i < ROWS; ++i) {
            (*s)[instruction->a][i] = temp[i];
        }
    } else {
        assert(false);
    }
}

static int countLit(screenptr s) {
    int total = 0;
    for (int i = 0; i < COLS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            if ((*s)[i][j]) {
                total += 1;
            }
        }
    }
    return total;
}

FUNCTION_DEFN_FOR_YDS(2016, eight, a) {
    screen s = {0};
    readInput(fp, handlerA, s);
    printf("%d\n", countLit(&s));
}

static void printScreen(screenptr s) {
    for (int i = 0; i < COLS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            char c = (*s)[i][j] ? '#' : ' ';
            putchar(c);
        }
        puts("");
    }
}

FUNCTION_DEFN_FOR_YDS(2016, eight, b) {
    screen s = {0};
    readInput(fp, handlerA, s);
    printScreen(&s);
}
