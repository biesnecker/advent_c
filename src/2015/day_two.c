#include "../common.h"

static int min(int a, int b) {
    return a < b ? a : b;
}

FUNCTION_DEFN_FOR_YDS(2015, two, a) {
    int length, width, height;

    int total = 0;

    while (fscanf(fp, "%dx%dx%d", &length, &width, &height) != EOF) {
        int side_a = length * width;
        int side_b = length * height;
        int side_c = width * height;
        total += (2 * side_a) + (2 * side_b) + (2 * side_c);
        total += min(min(side_a, side_b), side_c);
    }
    printf("%d\n", total);
}

static int perimeter(int a, int b) {
    return (2 * a) + (2 * b);
}

FUNCTION_DEFN_FOR_YDS(2015, two, b) {
    int length, width, height;

    int total = 0;

    while (fscanf(fp, "%dx%dx%d", &length, &width, &height) != EOF) {
        // The bow
        total += length * width * height;

        total += min(perimeter(length, width),
                     min(perimeter(length, height), perimeter(width, height)));
    }
    printf("%d\n", total);
}
