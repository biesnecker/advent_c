#include "../common.h"

static void readInput(FILE* fp, void (*handler)(int, void*), void* userData) {
    char buffer[10];
    int sum = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (buffer[0] == '\n') {
            handler(sum, userData);
            sum = 0;
        } else {
            sum += (int)strtol(buffer, NULL, 10);
        }
    }
    if (sum > 0) {
        handler(sum, userData);
    }
}

static void handlerA(int n, void* userData) {
    int* max = (int*)userData;

    if (n > *max) {
        *max = n;
    }
}

static void handlerB(int n, void* userData) {
    int* top = (int*)userData;
    for (int idx = 0; idx < 3; ++idx) {
        if (n > top[idx]) {
            int tmp = top[idx];
            top[idx] = n;
            n = tmp;
        }
    }
}

FUNCTION_DEFN_FOR_YDS(2022, one, a) {
    int max = 0;
    readInput(fp, handlerA, &max);
    printf("%d\n", max);
}

FUNCTION_DEFN_FOR_YDS(2022, one, b) {
    int top[3] = {-1, -1, -1};
    readInput(fp, handlerB, top);
    printf("%d\n", top[0] + top[1] + top[2]);
}
