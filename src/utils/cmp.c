#include "cmp.h"

int cmp_int_asc(const void* a, const void* b) {
    return (*(const int*)a - *(const int*)b);
}
int cmp_int_dsc(const void* a, const void* b) {
    return (*(const int*)b - *(const int*)a);
}
