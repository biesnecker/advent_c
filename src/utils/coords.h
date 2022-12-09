#pragma once
#include <stdbool.h>

typedef struct {
    int x, y;
} coord;

void coord_up(coord*);
void coord_down(coord*);
void coord_left(coord*);
void coord_right(coord*);

bool coord_adjacent(const coord*, const coord*);

int manhattan_distance(const coord*, const coord*);

// For sort functions, etc. Sorts bottom left (-x, -y) to upper right (+x, +y).
int coord_cmp(const void* x, const void* y);
