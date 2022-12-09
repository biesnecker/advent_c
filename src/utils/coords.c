#include "coords.h"

#include <stdlib.h>

void coord_up(coord* c) {
    c->y += 1;
}

void coord_down(coord* c) {
    c->y -= 1;
}

void coord_left(coord* c) {
    c->x -= 1;
}

void coord_right(coord* c) {
    c->x += 1;
}

bool coord_adjacent(const coord* a, const coord* b) {
    return abs(a->x - b->x) < 2 && abs(a->y - b->y) < 2;
}

int manhattan_distance(const coord* a, const coord* b) {
    return abs(a->x - b->x) + abs(a->y - b->y);
}

int coord_cmp(const void* x, const void* y) {
    const coord* a = (const coord*)x;
    const coord* b = (const coord*)y;

    if (a->x == b->x) {
        return a->y == b->y ? 0 : a->y > b->y ? 1 : -1;
    }
    return a->x > b->x ? 1 : -1;
}
