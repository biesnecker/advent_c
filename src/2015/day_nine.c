#include "../common.h"
#include "../utils/minmax.h"

static const int MAX_CITIES = 8;
static const int MAX_CITY_LENGTH = 50;

static int getCityId(char cities[MAX_CITIES][MAX_CITY_LENGTH],
                     const char* city) {
    for (int i = 0; i < MAX_CITIES; ++i) {
        if (cities[i][0] == '\0') {
            // we've not seen this city before.
            strcpy(cities[i], city);
            return i;
        } else if (strcmp(cities[i], city) == 0) {
            return i;
        }
    }
    // Should be exhaustive.
    assert(false);
    return -1;
}

static void parseInput(FILE* fp, int distances[MAX_CITIES][MAX_CITIES]) {
    char cities[MAX_CITIES][MAX_CITY_LENGTH];
    memset(cities, 0, sizeof(cities));

    char cityA[50] = {0};
    char cityB[50] = {0};
    int distance = 0;
    while (fscanf(fp, "%s to %s = %d\n", cityA, cityB, &distance) != EOF) {
        int cityIdA = getCityId(cities, cityA);
        int cityIdB = getCityId(cities, cityB);
        distances[cityIdA][cityIdB] = distance;
        distances[cityIdB][cityIdA] = distance;
    }
}

static int permute(int distances[MAX_CITIES][MAX_CITIES],
                   int lastCity,
                   int total,
                   uint8_t mask,
                   int (*cmp)(int, int),
                   int base) {
    int answer = base;
    if (mask == 0xff) {
        return total;
    }
    for (int i = 0; i < MAX_CITIES; ++i) {
        uint8_t check = 1U << i;
        if (mask & check) {
            // Already seen.
            continue;
        }

        int distance;
        if (lastCity == -1) {
            // this is the first city, so don't add last city distance to total.
            distance = permute(distances, i, total, mask | check, cmp, base);
        } else {
            distance = permute(distances,
                               i,
                               total + distances[lastCity][i],
                               mask | check,
                               cmp,
                               base);
        }
        answer = cmp(distance, answer);
    }
    return answer;
}

FUNCTION_DEFN_FOR_YDS(2015, nine, a) {
    int distances[MAX_CITIES][MAX_CITIES];
    memset(distances, 0, sizeof(distances));

    parseInput(fp, distances);

    int solution = permute(distances, -1, 0, 0, min, INT_MAX);
    printf("%d\n", solution);
}

FUNCTION_DEFN_FOR_YDS(2015, nine, b) {
    int distances[MAX_CITIES][MAX_CITIES];
    memset(distances, 0, sizeof(distances));

    parseInput(fp, distances);

    int solution = permute(distances, -1, 0, 0, max, INT_MIN);
    printf("%d\n", solution);
}
