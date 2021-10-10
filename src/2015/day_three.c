#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#include "../macros.h"
#include "../utils/minmax.h"

static uint16_t fromCoords(int x, int y) {
    uint16_t x_ = (uint8_t)x;
    uint16_t y_ = (uint8_t)y;
    return (x_ << 8) + y_;
}

FUNCTION_DEFN_FOR_YDS(2015, three, a) {
    char c;
    int xpos = 0;
    int ypos = 0;

    bool cells[UINT16_MAX] = {false};
    cells[fromCoords(0, 0)] = true;
    int count = 1;

    while (true) {
        c = (char)fgetc(fp);
        if (c == EOF) {
            break;
        }
        switch (c) {
        case '<':
            --xpos;
            break;
        case '>':
            ++xpos;
            break;
        case '^':
            ++ypos;
            break;
        case 'v':
            --ypos;
            break;
        }
        uint16_t coord = fromCoords(xpos, ypos);
        if (!cells[coord]) {
            ++count;
            cells[coord] = true;
        }
    }
    printf("%d\n", count);
}

FUNCTION_DEFN_FOR_YDS(2015, three, b) {
    char c;
    int santaXpos = 0;
    int santaYpos = 0;
    int robotXpos = 0;
    int robotYpos = 0;

    bool cells[UINT16_MAX] = {false};
    cells[fromCoords(0, 0)] = true;
    int count = 1;

    bool isSanta = true;

    while (true) {
        c = (char)fgetc(fp);
        if (c == EOF) {
            break;
        }
        int* xpos;
        int* ypos;
        if (isSanta) {
            xpos = &santaXpos;
            ypos = &santaYpos;
        } else {
            xpos = &robotXpos;
            ypos = &robotYpos;
        }
        isSanta = !isSanta;
        switch (c) {
        case '<':
            --(*xpos);
            break;
        case '>':
            ++(*xpos);
            break;
        case '^':
            ++(*ypos);
            break;
        case 'v':
            --(*ypos);
            break;
        }
        uint16_t coord = fromCoords(*xpos, *ypos);
        if (!cells[coord]) {
            ++count;
            cells[coord] = true;
        }
    }
    printf("%d\n", count);
}
