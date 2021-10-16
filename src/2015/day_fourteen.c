#include "../common.h"
#include "../utils/minmax.h"

typedef struct racer {
    char name[50];
    int flyingTime;
    int restingTime;
    int speed;
    bool isFlying;
    int lastTransition;
    int totalDistance;
    int totalScore;
} racer;

static void parseInput(FILE* fp, racer* racers) {
    racer* currentRacer = racers;
    while (fscanf(fp,
                  "%s can fly %d km/s for %d seconds, but then must rest for "
                  "%d seconds.\n",
                  currentRacer->name,
                  &currentRacer->speed,
                  &currentRacer->flyingTime,
                  &currentRacer->restingTime) != EOF) {
        currentRacer->isFlying = true;
        currentRacer->lastTransition = 0;
        currentRacer->totalDistance = 0;
        currentRacer->totalScore = 0;
        ++currentRacer;
    }
}

static void race(racer* racers, int nRacers, int rounds) {
    for (int t = 0; t < rounds; ++t) {
        int maxDistance = 0;
        for (int i = 0; i < nRacers; ++i) {
            // Should we transition?
            if (racers[i].isFlying) {
                if (t - racers[i].lastTransition == racers[i].flyingTime) {
                    racers[i].isFlying = false;
                    racers[i].lastTransition = t;
                }
            } else {
                if (t - racers[i].lastTransition == racers[i].restingTime) {
                    racers[i].isFlying = true;
                    racers[i].lastTransition = t;
                }
            }
            if (racers[i].isFlying) {
                racers[i].totalDistance += racers[i].speed;
            }
            maxDistance = max(maxDistance, racers[i].totalDistance);
        }
        // Assign points to any racer current leadiner.
        for (int i = 0; i < nRacers; ++i) {
            if (racers[i].totalDistance == maxDistance) {
                racers[i].totalScore += 1;
            }
        }
    }
}

FUNCTION_DEFN_FOR_YDS(2015, fourteen, a) {
    racer reindeers[9] = {0};
    parseInput(fp, reindeers);

    race(reindeers, 9, 2503);
    int winner = 0;
    for (int i = 0; i < 9; ++i) {
        winner = max(winner, reindeers[i].totalDistance);
    }
    printf("%d\n", winner);
}

FUNCTION_DEFN_FOR_YDS(2015, fourteen, b) {
    racer reindeers[9] = {0};
    parseInput(fp, reindeers);

    race(reindeers, 9, 2503);
    int winner = 0;
    for (int i = 0; i < 9; ++i) {
        winner = max(winner, reindeers[i].totalScore);
    }
    printf("%d\n", winner);
}
