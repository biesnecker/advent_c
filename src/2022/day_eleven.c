#include "../common.h"
#include "../utils/cmp.h"
#include "../utils/queue.h"

#define N_MONKIES 8

QUEUE_DEFINE(queue, int64_t, 64);

typedef enum {
    ADD,
    MUL,
} op;

typedef struct {
    queue items;
    op op;
    int left, right;
    int test, onTrue, onFalse;
} monkey;

static void readInput(FILE* fp, monkey* monkies) {
    UNUSED_ARG(monkies);
    char buffer[128];
    int line = 0;
    int monkeyId = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        switch (line % 7) {
        case 0: {
            // Monkey number
            assert(buffer[0] == 'M');
            break;
        }
        case 1: {
            // Starting items
            assert(buffer[2] == 'S');
            int item = 0;
            char* currentToken = strtok(buffer + 18, ",");
            while (currentToken != NULL) {
                assert(sscanf(currentToken, "%d", &item) == 1);
                queue_push(&monkies[monkeyId].items, item);
                currentToken = strtok(NULL, ",");
            }
            break;
        }
        case 2: {
            // Operation
            assert(buffer[2] == 'O');
            char* s = buffer + 19;
            char* opt = strchr(s, '+');
            if (opt == NULL) {
                opt = strchr(s, '*');
                assert(opt != NULL);
                monkies[monkeyId].op = MUL;
            } else {
                monkies[monkeyId].op = ADD;
            }
            if (s[0] == 'o') {
                monkies[monkeyId].left = -1;
            } else {
                monkies[monkeyId].left = (int)strtol(s, &opt, 10);
            }
            s = opt + 2;
            if (s[0] == 'o') {
                monkies[monkeyId].right = -1;
            } else {
                monkies[monkeyId].right = (int)strtol(s, NULL, 10);
            }
            break;
        }
        case 3: {
            // Divisible test
            assert(buffer[2] == 'T');
            monkies[monkeyId].test = (int)strtol(buffer + 21, NULL, 10);
            break;
        }
        case 4: {
            // On true
            assert(sscanf(buffer,
                          "  If true: throw to monkey %d",
                          &monkies[monkeyId].onTrue) == 1);
            break;
        }
        case 5: {
            // On false
            assert(sscanf(buffer,
                          "  If false: throw to monkey %d",
                          &monkies[monkeyId].onFalse) == 1);
            break;
        }
        case 6: {
            // Blank line between monkies
            monkeyId += 1;
        }
        }
        line += 1;
    }
}

static int64_t updateItem(int64_t original, op op, int left, int right) {
    int64_t a = (int64_t)(left == -1 ? original : left);
    int64_t b = (int64_t)(right == -1 ? original : right);

    return op == ADD ? a + b : a * b;
}

static int64_t runSimulation(monkey* monkies, int nRounds, bool partB) {
    int64_t modulo = 1;
    for (int i = 0; i < N_MONKIES; ++i) {
        modulo *= monkies[i].test;
    }
    int inspectCount[N_MONKIES] = {0};
    for (int round = 0; round < nRounds; ++round) {
        for (int mid = 0; mid < N_MONKIES; ++mid) {
            monkey* m = &monkies[mid];
            while (!queue_empty(&m->items)) {
                inspectCount[mid] += 1;
                const int64_t item = queue_pop(&m->items);
                int64_t newItem = updateItem(item, m->op, m->left, m->right);
                if (partB) {
                    newItem %= modulo;
                } else {
                    newItem /= 3;
                }
                assert(newItem <= INT_MAX);
                if (newItem % monkies[mid].test == 0) {
                    queue_push(&monkies[m->onTrue].items, (int)newItem);
                } else {
                    queue_push(&monkies[m->onFalse].items, (int)newItem);
                }
            }
        }
    }
    qsort(inspectCount, N_MONKIES, sizeof(int), cmp_int_dsc);
    int64_t product = 1;
    for (int i = 0; i < 2; ++i) {
        product *= (int64_t)inspectCount[i];
    }
    return product;
}

FUNCTION_DEFN_FOR_YDS(2022, eleven, a) {
    monkey monkies[N_MONKIES] = {0};
    readInput(fp, monkies);

    int64_t res = runSimulation(monkies, 20, false /* part B*/);
    printf("%lld\n", res);
}

FUNCTION_DEFN_FOR_YDS(2022, eleven, b) {
    monkey monkies[N_MONKIES] = {0};
    readInput(fp, monkies);

    int64_t res = runSimulation(monkies, 10000, true /* part B*/);
    printf("%lld\n", res);
}
