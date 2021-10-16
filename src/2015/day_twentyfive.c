#include "../common.h"

static void parseInput(FILE* fp, uint32_t* row, uint32_t* col) {
    char buffer[125];
    assert(fgets(buffer, sizeof(buffer), fp) != NULL);

    char* start = strstr(buffer, "row");
    assert(start != NULL);

    assert(sscanf(start, "row %u, column %u.", row, col) == 2);
}

static uint64_t
findCode(uint32_t expectedRow, uint32_t expectedColumn, uint64_t seed) {
    uint32_t row = 2, col = 1;
    while (true) {
        uint32_t crow = row, ccol = col;
        do {
            seed = (seed * 252533) % 33554393;
            if (crow == expectedRow && ccol == expectedColumn) {
                return seed;
            }
            crow -= 1;
            ccol += 1;
        } while (crow > 0);
        row += 1;
    }
    return 0;
}

FUNCTION_DEFN_FOR_YDS(2015, twentyfive, a) {
    uint32_t row, col;
    parseInput(fp, &row, &col);

    uint64_t solution = findCode(row, col, 20151125);

    printf("%llu\n", solution);
}

FUNCTION_DEFN_FOR_YDS(2015, twentyfive, b) {
    UNUSED_ARG(fp);
    printf("There is no problem 25B ::shrug::\n");
}
