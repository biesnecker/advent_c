#include "../common.h"
#include "../utils/cmp.h"

#define MAX_INPUT_LEN 300

static int readInput(FILE* fp, int* nums) {
    char buffer[50];
    int idx = 0;
    int sum = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (buffer[0] == '\n') {
            nums[idx++] = sum;
            sum = 0;
        } else {
            sum += (int)strtol(buffer, NULL, 10);
        }
    }
    return idx;
}

FUNCTION_DEFN_FOR_YDS(2022, one, a) {
    int elves[MAX_INPUT_LEN] = {0};
    const int nElves = readInput(fp, elves);
    int max = 0;
    for (int idx = 0; idx < nElves; ++idx) {
        if (elves[idx] > max) {
            max = elves[idx];
        }
    }
    printf("%d\n", max);
}

FUNCTION_DEFN_FOR_YDS(2022, one, b) {
    int elves[MAX_INPUT_LEN] = {0};
    const int nElves = readInput(fp, elves);

    qsort(elves, nElves, sizeof(elves[0]), cmp_int_dsc);

    int sum = elves[0] + elves[1] + elves[2];

    printf("%d\n", sum);
}
