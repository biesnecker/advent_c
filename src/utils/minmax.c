#include "minmax.h"

#include <assert.h>

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int clamp(int val, int lower, int upper) {
    assert(lower <= upper);
    return max(min(val, upper), lower);
}
