#include "../common.h"

#define INPUT_SIZE 2000

static void readInput(FILE* fp, void (*handler)(int*)) {
    char buffer[20] = {0};
    int input[INPUT_SIZE] = {0};
    int idx = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        int n;
        assert(sscanf(buffer, "%d", &n) == 1);
        input[idx++] = n;
    }

    handler(input);
}

static void handlerA(int* n) {
    int count = 0;
    for (int i = 1; i < INPUT_SIZE; ++i) {
        if (n[i - 1] < n[i]) {
            ++count;
        }
    }
    printf("%d\n", count);
}

static void handlerB(int* n) {
    int count = 0;
    for (int i = 3; i < INPUT_SIZE; ++i) {
        if (n[i - 3] < n[i]) {
            ++count;
        }
    }
    printf("%d\n", count);
}

FUNCTION_DEFN_FOR_YDS(2021, one, a) {
    readInput(fp, handlerA);
}

FUNCTION_DEFN_FOR_YDS(2021, one, b) {
    readInput(fp, handlerB);
}
