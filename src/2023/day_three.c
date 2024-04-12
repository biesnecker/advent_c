#include "../common.h"

#define MAX_NUMS 2048
#define MAX_SYMBOLS 2048

typedef struct _Symbol {
    char s;
    int y;
    int x;
} Symbol;

typedef struct _Num {
    int n;
    int y;
    int begin;
    int end;
} Num;

typedef struct _Input {
    Num nums[MAX_NUMS];
    Symbol symbols[MAX_SYMBOLS];
    int symC;
    int numC;
} Input;

static void readInput(FILE* fp, Input* input) {
    char buffer[200];
    int y = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        const char* p = buffer;
        int current = -1;
        int currentLen = 0;
        int x = 0;
        while (*p && *p != '\n') {
            if (isdigit(*p)) {
                if (current < 0) {
                    current = *p - '0';
                } else {
                    current *= 10;
                    current += *p - '0';
                }
                ++currentLen;
            } else {
                if (current >= 0) {
                    input->nums[input->numC++] =
                        (Num){current, y, x - currentLen, x - 1};
                    current = -1;
                    currentLen = 0;
                }
                if (*p != '.') {
                    input->symbols[input->symC++] = (Symbol){*p, y, x};
                }
            }
            ++x;
            ++p;
        }
        if (current >= 0) {
            input->nums[input->numC++] =
                (Num){current, y, x - currentLen, x - 1};
        }

        ++y;
    }
}

static int adjacent(const Symbol* s, const Num* n) {
    return abs(n->y - s->y) <= 1 && n->begin - 1 <= s->x && s->x <= n->end + 1;
}

FUNCTION_DEFN_FOR_YDS(2023, three, a) {
    Input in = {0};
    readInput(fp, &in);

    int s = 0;

    for (int i = 0; i < in.numC; ++i) {
        for (int j = 0; j < in.symC; ++j) {
            if (adjacent(&in.symbols[j], &in.nums[i])) {
                s += in.nums[i].n;
                break;
            }
        }
    }
    printf("%d\n", s);
}

FUNCTION_DEFN_FOR_YDS(2023, three, b) {
    Input in = {0};
    readInput(fp, &in);

    int s = 0;

    for (int i = 0; i < in.symC; ++i) {
        if (in.symbols[i].s != '*') {
            continue;
        }
        int cnt = 0;
        int p = 1;
        for (int j = 0; j < in.numC; ++j) {
            if (in.nums[j].y > in.symbols[i].y + 1) {
                break;
            }
            if (adjacent(&in.symbols[i], &in.nums[j])) {
                ++cnt;
                p *= in.nums[j].n;
            }
        }
        if (cnt == 2) {
            s += p;
        }
    }
    printf("%d\n", s);
}
