#include "../common.h"

static void readInput(FILE* fp, int* hitpoints, int* damage) {
    assert(fscanf(fp, "Hit Points: %d\n", hitpoints) == 1);
    assert(fscanf(fp, "Damage: %d\n", damage) == 1);
}

typedef enum action {
    NOTHING = 0,
    MAGIC_MISSILE,
    DRAIN,
    SHIELD,
    POISON,
    RECHARGE,
    _ACTION_COUNT,
} action;

static const int actionCosts[_ACTION_COUNT] = {0, 53, 73, 113, 173, 229};

typedef struct effects {
    uint8_t shield;
    uint8_t poison;
    uint8_t recharge;
} effects;

typedef struct gamestate {
    int playerHp;
    int playerMana;
    int bossHp;
    int bossDamage;
    int totalManaSpent;
    effects eff;
} gamestate;

static void handleEffects(gamestate* state, bool* hasArmor) {
    *hasArmor = false;
    if (state->eff.poison > 0) {
        state->bossHp -= 3;
        state->eff.poison -= 1;
    }
    if (state->eff.recharge > 0) {
        state->playerMana += 101;
        state->eff.recharge -= 1;
    }
    if (state->eff.shield > 0) {
        *hasArmor = true;
        state->eff.shield -= 1;
    }
}

static void
simulate(gamestate* state, bool isPlayerTurn, int* minMana, bool hardMode) {
    if (state->totalManaSpent > *minMana) {
        return;
    }
    if (hardMode && isPlayerTurn) {
        state->playerHp -= 1;
        if (state->playerHp <= 0) {
            return;
        }
    }
    // Handle effects
    bool hasArmor = false;
    handleEffects(state, &hasArmor);

    // Check if the boss died after effects.
    if (state->bossHp <= 0) {
        if (*minMana > state->totalManaSpent) {
            *minMana = state->totalManaSpent;
        }
        return;
    }

    if (isPlayerTurn) {
        // Player turn. Need to choose an action.
        bool didAction = false;
        for (int a = MAGIC_MISSILE; a < _ACTION_COUNT; ++a) {
            int manaCost = actionCosts[a];
            gamestate newState;
            if (state->playerMana < manaCost) {
                // Not enough manage for this action.
                continue;
            } else if (a == SHIELD && state->eff.shield > 0) {
                continue;
            } else if (a == POISON && state->eff.poison > 0) {
                continue;
            } else if (a == RECHARGE && state->eff.recharge > 0) {
                continue;
            }

            didAction = true;

            // Done all the checks for this action. Now copy the state into the
            // local, update it based on the action, and recurse.
            memcpy(&newState, state, sizeof(gamestate));
            newState.playerMana -= manaCost;
            assert(newState.playerMana >= 0);
            newState.totalManaSpent += manaCost;
            switch (a) {
            case MAGIC_MISSILE:
                newState.bossHp -= 4;
                break;
            case DRAIN:
                newState.bossHp -= 2;
                newState.playerHp += 2;
                break;
            case SHIELD:
                newState.eff.shield = 6;
                break;
            case POISON:
                newState.eff.poison = 6;
                break;
            case RECHARGE:
                newState.eff.recharge = 5;
                break;
            default:
                assert(false);
            }

            // Check if the boss died after effects.
            if (newState.bossHp <= 0) {
                if (*minMana > newState.totalManaSpent) {
                    *minMana = newState.totalManaSpent;
                }
                return;
            } else {
                // Run the boss's turn.
                simulate(&newState, false, minMana, hardMode);
            }
        }

        if (!didAction) {
            // You ran out of mana.
            return;
        }
    } else {
        // Boss turn. We can directly update the state because it came from
        // the last iteration's loop local value.
        int armor = hasArmor ? 7 : 0;
        int damage = state->bossDamage - armor;
        state->playerHp -= (damage > 0 ? damage : 1);
        if (state->playerHp <= 0) {
            // You're dead.
            return;
        } else {
            simulate(state, true, minMana, hardMode);
        }
    }
}

FUNCTION_DEFN_FOR_YDS(2015, twentytwo, a) {
    int bossHitpoints;
    int bossDamage;

    readInput(fp, &bossHitpoints, &bossDamage);

    gamestate gs = {.bossHp = bossHitpoints,
                    .bossDamage = bossDamage,
                    .playerHp = 50,
                    .playerMana = 500,
                    .totalManaSpent = 0,
                    .eff = {0}};

    int minMana = INT_MAX;
    simulate(&gs, true, &minMana, false);

    printf("%d\n", minMana);
}

FUNCTION_DEFN_FOR_YDS(2015, twentytwo, b) {
    int bossHitpoints;
    int bossDamage;

    readInput(fp, &bossHitpoints, &bossDamage);

    gamestate gs = {.bossHp = bossHitpoints,
                    .bossDamage = bossDamage,
                    .playerHp = 50,
                    .playerMana = 500,
                    .totalManaSpent = 0,
                    .eff = {0}};

    int minMana = INT_MAX;
    simulate(&gs, true, &minMana, true);

    printf("%d\n", minMana);
}
