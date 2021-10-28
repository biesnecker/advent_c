#include "../common.h"
#include "../utils/minmax.h"

typedef struct indgredient {
    char name[50];
    int capacity;
    int durability;
    int flavor;
    int texture;
    int calories;
} ingredient;

static void parseInput(FILE* fp, ingredient* ingredients) {
    ingredient* i = ingredients;
    while (fscanf(fp,
                  "%s capacity %d, durability %d, flavor %d, texture %d, "
                  "calories %d \n",
                  i->name,
                  &i->capacity,
                  &i->durability,
                  &i->flavor,
                  &i->texture,
                  &i->calories) != EOF) {
        ++i;
    }
}

static int scoreCookie(ingredient* ingredients, int i, int j, int k, int m) {
    int capacity =
        max(0,
            (ingredients[0].capacity * i) + (ingredients[1].capacity * j) +
                (ingredients[2].capacity * k) + (ingredients[3].capacity * m));
    int durability = max(
        0,
        (ingredients[0].durability * i) + (ingredients[1].durability * j) +
            (ingredients[2].durability * k) + (ingredients[3].durability * m));
    int flavor =
        max(0,
            (ingredients[0].flavor * i) + (ingredients[1].flavor * j) +
                (ingredients[2].flavor * k) + (ingredients[3].flavor * m));
    int texture =
        max(0,
            (ingredients[0].texture * i) + (ingredients[1].texture * j) +
                (ingredients[2].texture * k) + (ingredients[3].texture * m));
    return capacity * durability * flavor * texture;
}

FUNCTION_DEFN_FOR_YDS(2015, fifteen, a) {
    ingredient ingredients[4] = {0};
    int maxScore = 0;
    int iters = 0;
    parseInput(fp, ingredients);
    for (int i = 100; i >= 0; --i) {
        for (int j = 100 - i; j >= 0; --j) {
            for (int k = 100 - (i + j); k >= 0; --k) {
                int m = 100 - (i + j + k);
                ++iters;
                maxScore = max(maxScore, scoreCookie(ingredients, i, j, k, m));
            }
        }
    }
    printf("%d %d\n", iters, maxScore);
}

FUNCTION_DEFN_FOR_YDS(2015, fifteen, b) {
    ingredient ingredients[4] = {0};
    int maxScore = 0;
    parseInput(fp, ingredients);
    for (int i = 100; i >= 0; --i) {
        for (int j = 100 - i; j >= 0; --j) {
            for (int k = 100 - (i + j); k >= 0; --k) {
                int m = 100 - (i + j + k);
                int calories = (ingredients[0].calories * i) +
                               (ingredients[1].calories * j) +
                               (ingredients[2].calories * k) +
                               (ingredients[3].calories * m);
                if (calories == 500) {
                    maxScore =
                        max(maxScore, scoreCookie(ingredients, i, j, k, m));
                }
            }
        }
    }
    printf("%d\n", maxScore);
}
