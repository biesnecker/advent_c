#include "../common.h"

static void readInput(FILE* fp,
                      void (*handler)(int, int, int, int, void*),
                      void* userData) {
    char buffer[64];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        int a, b, c, d;

        assert(sscanf(buffer, "%d-%d,%d-%d", &a, &b, &c, &d) == 4);
        handler(a, b, c, d, userData);
    }
}

static void handlerA(int a, int b, int c, int d, void* userData) {
    int* total = (int*)userData;

    if ((c >= a && d <= b) || (a >= c && b <= d)) {
        *total += 1;
    }
}

static void handlerB(int a, int b, int c, int d, void* userData) {
    int* total = (int*)userData;

    if ((c >= a && c <= b) || (a >= c && a <= d)) {
        *total += 1;
    }
}

FUNCTION_DEFN_FOR_YDS(2022, four, a) {
    int total = 0;
    readInput(fp, handlerA, &total);
    printf("%d\n", total);
}

FUNCTION_DEFN_FOR_YDS(2022, four, b) {
    int total = 0;
    readInput(fp, handlerB, &total);
    printf("%d\n", total);
}
