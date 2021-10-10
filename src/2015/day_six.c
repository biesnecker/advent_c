#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../macros.h"
#include "../utils/minmax.h"
#include "../utils/strings.h"

typedef enum mode { TURNON, TURNOFF, TOGGLE } mode;

FUNCTION_DEFN_FOR_YDS(2015, six, a) {
    bool grid[1000][1000] = {false};
    char line[100] = {0};

    int startX, startY, endX, endY;

    while (fgets(line, sizeof(line), fp) != NULL) {
        mode m;
        if (isPrefix("turn on", line)) {
            sscanf(line,
                   "turn on %d,%d through %d,%d",
                   &startX,
                   &startY,
                   &endX,
                   &endY);
            m = TURNON;
        } else if (isPrefix("turn off", line)) {
            sscanf(line,
                   "turn off %d,%d through %d,%d",
                   &startX,
                   &startY,
                   &endX,
                   &endY);
            m = TURNOFF;
        } else if (isPrefix("toggle", line)) {
            sscanf(line,
                   "toggle %d,%d through %d,%d",
                   &startX,
                   &startY,
                   &endX,
                   &endY);
            m = TOGGLE;
        }
        for (int x = startX; x <= endX; ++x) {
            for (int y = startY; y <= endY; ++y) {
                switch (m) {
                case TURNON:
                    grid[x][y] = true;
                    break;
                case TURNOFF:
                    grid[x][y] = false;
                    break;
                case TOGGLE:
                    grid[x][y] = !grid[x][y];
                    break;
                }
            }
        }
    }
    int c = 0;
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1000; ++j) {
            if (grid[i][j]) {
                ++c;
            }
        }
    }
    printf("%d\n", c);
}

FUNCTION_DEFN_FOR_YDS(2015, six, b) {
    int grid[1000][1000] = {0};
    char line[100] = {0};

    int startX, startY, endX, endY;

    while (fgets(line, sizeof(line), fp) != NULL) {
        mode m;
        if (isPrefix("turn on", line)) {
            sscanf(line,
                   "turn on %d,%d through %d,%d",
                   &startX,
                   &startY,
                   &endX,
                   &endY);
            m = TURNON;
        } else if (isPrefix("turn off", line)) {
            sscanf(line,
                   "turn off %d,%d through %d,%d",
                   &startX,
                   &startY,
                   &endX,
                   &endY);
            m = TURNOFF;
        } else if (isPrefix("toggle", line)) {
            sscanf(line,
                   "toggle %d,%d through %d,%d",
                   &startX,
                   &startY,
                   &endX,
                   &endY);
            m = TOGGLE;
        }
        for (int x = startX; x <= endX; ++x) {
            for (int y = startY; y <= endY; ++y) {
                switch (m) {
                case TURNON:
                    grid[x][y] += 1;
                    break;
                case TURNOFF:
                    grid[x][y] = max(grid[x][y] - 1, 0);
                    break;
                case TOGGLE:
                    grid[x][y] += 2;
                    break;
                }
            }
        }
    }
    int c = 0;
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1000; ++j) {
            c += grid[i][j];
        }
    }
    printf("%d\n", c);
}
