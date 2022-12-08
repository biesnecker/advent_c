#include "../common.h"

#define ROWS 99
#define COLS 99
#define MAX_STACK 16
#define N_HEIGHTS 10

typedef struct {
    int index;
    int height;
} stack_elem;

typedef struct {
    stack_elem elems[MAX_STACK];
    int tos;
} stack;

typedef struct {
    int heights[N_HEIGHTS];
} last_seen_map;

static stack_elem stack_peek(const stack* s) {
    assert(s->tos > 0);
    return s->elems[s->tos - 1];
}

static void stack_push(stack* s, stack_elem elem) {
    assert(s->tos < MAX_STACK);
    s->elems[s->tos++] = elem;
}

static stack_elem stack_pop(stack* s) {
    assert(s->tos > 0);
    return s->elems[(s->tos--) - 1];
}

static bool stack_empty(const stack* s) {
    return s->tos == 0;
}

static int index_from_coords(int x, int y) {
    return (y * COLS) + x;
}

static int readInput(FILE* fp, int* map) {
    int cr = 0;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        for (int i = 0; i < COLS; i++) {
            *(map++) = buffer[i] - '0';
            cr += 1;
        }
    }
    return cr;
}

FUNCTION_DEFN_FOR_YDS(2022, eight, a) {
    int grid[ROWS * COLS] = {0};
    bool visible[ROWS * COLS] = {false};
    readInput(fp, grid);

    int fromTop[COLS] = {0};
    for (int i = 0; i < COLS; ++i) {
        fromTop[i] = -1;
    }
    stack fromBottom[COLS] = {0};

    for (int y = 0; y < ROWS; ++y) {
        int fromLeft = -1;
        stack fromRight = {0};
        for (int x = 0; x < COLS; ++x) {
            const int idx = index_from_coords(x, y);
            const int height = grid[idx];

            // Visible from the left.
            if (height > fromLeft) {
                visible[idx] = true;
                fromLeft = height;
            }

            // Visible from the top.
            if (height > fromTop[x]) {
                visible[idx] = true;
                fromTop[x] = height;
            }

            // Handle the row stack.
            while (!stack_empty(&fromRight) &&
                   stack_peek(&fromRight).height <= height) {
                stack_pop(&fromRight);
            }
            stack_push(&fromRight, (stack_elem){.index = x, .height = height});

            // Handle the column stack.
            stack* colStack = &fromBottom[x];
            while (!stack_empty(colStack) &&
                   stack_peek(colStack).height <= height) {
                stack_pop(colStack);
            }
            stack_push(colStack, (stack_elem){.index = y, .height = height});
        }
        // Clean up the row stack at the end of the row.
        while (!stack_empty(&fromRight)) {
            int x = stack_pop(&fromRight).index;
            visible[index_from_coords(x, y)] = true;
        }
    }
    // Clean up the column stacks at the end of all the rows.
    for (int x = 0; x < COLS; ++x) {
        stack* colStack = &fromBottom[x];
        while (!stack_empty(colStack)) {
            int y = stack_pop(colStack).index;
            visible[index_from_coords(x, y)] = true;
        }
    }

    int totalVisible = 0;
    for (int i = 0; i < ROWS * COLS; ++i) {
        if (visible[i]) {
            ++totalVisible;
        }
    }
    printf("%d\n", totalVisible);
}

FUNCTION_DEFN_FOR_YDS(2022, eight, b) {
    int grid[ROWS * COLS] = {0};
    int visibility[ROWS * COLS];
    for (int i = 0; i < ROWS * COLS; ++i) {
        visibility[i] = 1;
    }
    readInput(fp, grid);

    stack fromBottom[COLS] = {0};
    last_seen_map lastSeenFromTop[COLS] = {0};

    for (int y = 0; y < ROWS; ++y) {
        last_seen_map lastSeenFromLeft = {0};
        stack fromRight = {0};
        for (int x = 0; x < COLS; ++x) {
            const int idx = index_from_coords(x, y);
            const int height = grid[idx];

            // Handle left visibility.
            const int lastEqualOrTallerFromLeftIdx =
                lastSeenFromLeft.heights[height];
            visibility[idx] *= (x - lastEqualOrTallerFromLeftIdx);
            for (int i = 0; i <= height; ++i) {
                lastSeenFromLeft.heights[i] = x;
            }

            // Handle top visibility.
            const int lastEqualOrTallerFromTopIdx =
                lastSeenFromTop[x].heights[height];
            visibility[idx] *= (y - lastEqualOrTallerFromTopIdx);
            for (int i = 0; i <= height; ++i) {
                lastSeenFromTop[x].heights[i] = y;
            }

            // Handle row stack.
            while (!stack_empty(&fromRight) &&
                   stack_peek(&fromRight).height <= height) {
                const int shorterX = stack_pop(&fromRight).index;
                const int shorterIdx = index_from_coords(shorterX, y);
                visibility[shorterIdx] *= (x - shorterX);
            }
            stack_push(&fromRight, (stack_elem){.index = x, .height = height});

            // Handle column stack.
            stack* colStack = &fromBottom[x];
            while (!stack_empty(colStack) &&
                   stack_peek(colStack).height <= height) {
                const int shorterY = stack_pop(colStack).index;
                const int shorterIdx = index_from_coords(x, shorterY);
                visibility[shorterIdx] *= (y - shorterY);
            }
            stack_push(colStack, (stack_elem){.index = y, .height = height});
        }

        // Clean up the row stack at the end of the row.
        while (!stack_empty(&fromRight)) {
            const int x = stack_pop(&fromRight).index;
            const int idx = index_from_coords(x, y);
            visibility[idx] *= ((COLS - 1) - x);
        }
    }

    // Clean up the column stacks at the end of all the rows.
    for (int x = 0; x < COLS; ++x) {
        stack* colStack = &fromBottom[x];
        while (!stack_empty(colStack)) {
            const int y = stack_pop(colStack).index;
            const int idx = index_from_coords(x, y);
            visibility[idx] *= ((ROWS - 1) - y);
        }
    }

    int best = 0;
    for (int i = 0; i < ROWS * COLS; i++) {
        best = visibility[i] > best ? visibility[i] : best;
    }
    printf("%d\n", best);
}
