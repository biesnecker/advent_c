#include <inttypes.h>

#include "../common.h"

#define INPUT_SIZE 1000
#define BIT_LENGTH 12

static void readInput(FILE* fp, uint32_t* nums) {
    char buffer[20];
    int idx = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        nums[idx++] = (uint32_t)strtoul(buffer, NULL, 2);
    }
}

FUNCTION_DEFN_FOR_YDS(2021, three, a) {
    uint32_t nums[INPUT_SIZE] = {0};
    readInput(fp, nums);

    uint32_t gamma = 0;
    uint32_t epsilon = 0;

    for (int bit = 0; bit < BIT_LENGTH; ++bit) {
        int ones = 0;
        uint32_t mask = ((uint32_t)1) << bit;
        for (int idx = 0; idx < INPUT_SIZE; ++idx) {
            if (nums[idx] & mask) {
                ++ones;
            } else {
                --ones;
            }
        }
        if (ones > 0) {
            gamma |= mask;
        } else {
            epsilon |= mask;
        }
    }

    printf("%" PRIu32 "\n", gamma * epsilon);
}

FUNCTION_DEFN_FOR_YDS(2021, three, b) {
    UNUSED_ARG(fp);
}
