#include <stdlib.h>
#include <string.h>

#include "../common.h"

void readInput(FILE* fp, void (*handler)(const char*, void*), void* userData) {
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        handler(buffer, userData);
    }
}

void forEachGame(const char* buffer,
                 void (*handler)(int, int, int, int, void*),
                 void* userData) {
    char game[1024];
    int gameNum;

    if (sscanf(buffer, "Game %d: %[^\n]", &gameNum, game) != 2) {
        fprintf(stderr, "Failed to parse input\n");
        exit(1);
    }

    char* chunk;
    char* saveptr;
    char* chunkStr = game;
    while ((chunk = strtok_r(chunkStr, ";", &saveptr)) != NULL) {
        int red = 0, green = 0, blue = 0;
        char color[10];
        int count;

        char* colorCnt;
        char* colorSaveptr;
        char* colorStr = chunk;

        while ((colorCnt = strtok_r(colorStr, ",", &colorSaveptr)) != NULL) {
            if (sscanf(colorCnt, "%d %s", &count, color) != 2) {
                fprintf(stderr, "Failed to parse input\n");
                exit(1);
            }

            if (strcmp(color, "red") == 0) {
                red += count;
            } else if (strcmp(color, "green") == 0) {
                green += count;
            } else if (strcmp(color, "blue") == 0) {
                blue += count;
            } else {
                fprintf(stderr, "Unknown color: %s\n", color);
                exit(1);
            }

            colorStr = NULL;
        }

        handler(gameNum, red, green, blue, userData);
        chunkStr = NULL;
    }
}

void handlerAImpl(int gameNum, int red, int green, int blue, void* userData) {
    int* games = (int*)userData;
    if (red > 12 || green > 13 || blue > 14) {
        games[gameNum - 1] = 0;
    }
}

void handlerA(const char* buffer, void* userData) {
    forEachGame(buffer, handlerAImpl, userData);
}

void handlerBImpl(int gameNum, int red, int green, int blue, void* userData) {
    int* cubesNeeded = (int*)userData;
    int idx = (gameNum - 1) * 3;
    if (red > cubesNeeded[idx]) {
        cubesNeeded[idx] = red;
    }
    if (green > cubesNeeded[idx + 1]) {
        cubesNeeded[idx + 1] = green;
    }
    if (blue > cubesNeeded[idx + 2]) {
        cubesNeeded[idx + 2] = blue;
    }
}

void handlerB(const char* buffer, void* userData) {
    forEachGame(buffer, handlerBImpl, userData);
}

FUNCTION_DEFN_FOR_YDS(2023, two, a) {
    int ok[100] = {1};
    for (int i = 0; i < 100; ++i) {
        ok[i] = 1;
    }
    readInput(fp, handlerA, ok);
    int s = 0;
    for (int i = 0; i < 100; ++i) {
        if (ok[i]) {
            s += i + 1;
        }
    }
    printf("%d\n", s);
}

FUNCTION_DEFN_FOR_YDS(2023, two, b) {
    int cubesNeeded[300] = {0};
    readInput(fp, handlerB, cubesNeeded);
    int s = 0;
    for (int i = 0; i < 100; ++i) {
        int idx = i * 3;
        s += cubesNeeded[idx] * cubesNeeded[idx + 1] * cubesNeeded[idx + 2];
    }
    printf("%d\n", s);
}
