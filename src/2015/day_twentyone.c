#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#include "../macros.h"
#include "../utils/minmax.h"

typedef struct player {
    int hitpoints;
    int damage;
    int armor;
} player;

typedef struct item {
    int cost;
    int damage;
    int armor;
} item;

static const item weapons[] = {
    {.cost = 8, .damage = 4, .armor = 0},
    {.cost = 10, .damage = 5, .armor = 0},
    {.cost = 25, .damage = 6, .armor = 0},
    {.cost = 40, .damage = 7, .armor = 0},
    {.cost = 74, .damage = 8, .armor = 0},
};

static const int nWeapons = ARRAY_LEN(weapons);

static const item armors[] = {
    {.cost = 0, .damage = 0, .armor = 0},
    {.cost = 13, .damage = 0, .armor = 1},
    {.cost = 31, .damage = 0, .armor = 2},
    {.cost = 53, .damage = 0, .armor = 3},
    {.cost = 75, .damage = 0, .armor = 4},
    {.cost = 102, .damage = 0, .armor = 5},
};

static const int nArmors = ARRAY_LEN(armors);

static const item rings[] = {
    {.cost = 0, .damage = 0, .armor = 0},
    {.cost = 0, .damage = 0, .armor = 0},
    {.cost = 25, .damage = 1, .armor = 0},
    {.cost = 50, .damage = 2, .armor = 0},
    {.cost = 100, .damage = 3, .armor = 0},
    {.cost = 20, .damage = 0, .armor = 1},
    {.cost = 40, .damage = 0, .armor = 2},
    {.cost = 80, .damage = 0, .armor = 3},
};

static const int nRings = ARRAY_LEN(rings);

static int calcDamage(int damage, int armor) {
    return max(1, damage - armor);
}

static bool simulate(player* boss, player* you) {
    int youHp = you->hitpoints;
    int bossHp = boss->hitpoints;

    while (true) {
        bossHp -= calcDamage(you->damage, boss->armor);
        if (bossHp <= 0) {
            return true;
        }
        youHp -= calcDamage(boss->damage, you->armor);
        if (youHp <= 0) {
            return false;
        }
    }
}

static void play(player* boss,
                 int weapon,
                 int armor,
                 int leftRing,
                 int rightRing,
                 int* minCost,
                 int* maxCost) {
    if (weapon == -1) {
        for (int w = 0; w < nWeapons; ++w) {
            play(boss, w, armor, leftRing, rightRing, minCost, maxCost);
        }
        return;
    }
    if (armor == -1) {
        for (int a = 0; a < nArmors; ++a) {
            play(boss, weapon, a, leftRing, rightRing, minCost, maxCost);
        }
        return;
    }
    if (leftRing == -1) {
        for (int lr = 0; lr < nRings; ++lr) {
            if (lr == rightRing) {
                continue;
            }
            play(boss, weapon, armor, lr, rightRing, minCost, maxCost);
        }
        return;
    }
    if (rightRing == -1) {
        for (int rr = 0; rr < nRings; ++rr) {
            if (rr == leftRing) {
                continue;
            }
            play(boss, weapon, armor, leftRing, rr, minCost, maxCost);
        }
        return;
    }

    // actually sim the game.
    assert(weapon >= 0 && weapon < nWeapons);
    assert(armor >= 0 && armor < nArmors);
    assert(leftRing >= 0 && leftRing < nRings);
    assert(rightRing >= 0 && rightRing < nRings);
    assert(leftRing != rightRing);
    int totalCost = weapons[weapon].cost + armors[armor].cost +
                    rings[leftRing].cost + rings[rightRing].cost;
    player you = {
        .hitpoints = 100,
        .damage = weapons[weapon].damage + armors[armor].damage +
                  rings[leftRing].damage + rings[rightRing].damage,
        .armor = weapons[weapon].armor + armors[armor].armor +
                 rings[leftRing].armor + rings[rightRing].armor,
    };
    if (simulate(boss, &you)) {
        *minCost = min(*minCost, totalCost);
    } else {
        *maxCost = max(*maxCost, totalCost);
    }
}

static void readInput(FILE* fp, player* boss) {
    char buffer[50];
    int value;
    while (fscanf(fp, "%[a-zA-z ]: %d\n", buffer, &value) != EOF) {
        if (strcmp(buffer, "Hit Points") == 0) {
            boss->hitpoints = value;
        } else if (strcmp(buffer, "Damage") == 0) {
            boss->damage = value;
        } else if (strcmp(buffer, "Armor") == 0) {
            boss->armor = value;
        } else {
            assert(false);
        }
    }
}

FUNCTION_DEFN_FOR_YDS(2015, twentyone, a) {
    player boss = {0};
    readInput(fp, &boss);

    int minCost = INT_MAX;
    int maxCost = INT_MIN;

    play(&boss, -1, -1, -1, -1, &minCost, &maxCost);
    printf("%d\n", minCost);
}

FUNCTION_DEFN_FOR_YDS(2015, twentyone, b) {
    player boss = {0};
    readInput(fp, &boss);

    int minCost = INT_MAX;
    int maxCost = INT_MIN;

    play(&boss, -1, -1, -1, -1, &minCost, &maxCost);
    printf("%d\n", maxCost);
}
