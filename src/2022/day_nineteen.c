#include <assert.h>

#include "../common.h"

#define N_BLUEPRINTS 30

typedef struct {
    int blueprint_id;
    int ore_robot_cost;
    int clay_robot_cost;
    int obsidian_robot_cost_ore;
    int obsidian_robot_cost_clay;
    int geode_robot_cost_ore;
    int geode_robot_cost_obsidian;
} blueprint;

static blueprint blueprints[N_BLUEPRINTS];

static void readInput(FILE* fp) {
    for (int i = 0; i < N_BLUEPRINTS; ++i) {
        char buffer[512];
        fgets(buffer, sizeof(buffer), fp);
        assert(
            sscanf(buffer,
                   "Blueprint %d: Each ore robot costs %d ore. Each clay robot "
                   "costs %d ore. Each obsidian robot costs %d ore and %d "
                   "clay. Each geode robot costs %d ore and %d obsidian.",
                   &(blueprints[i].blueprint_id),
                   &(blueprints[i].ore_robot_cost),
                   &(blueprints[i].clay_robot_cost),
                   &(blueprints[i].obsidian_robot_cost_ore),
                   &(blueprints[i].obsidian_robot_cost_clay),
                   &(blueprints[i].geode_robot_cost_ore),
                   &(blueprints[i].geode_robot_cost_obsidian)) == 7);
    }
}

FUNCTION_DEFN_FOR_YDS(2022, nineteen, a) {
    UNUSED_ARG(fp);
};

FUNCTION_DEFN_FOR_YDS(2022, nineteen, b) {
    UNUSED_ARG(fp);
};
