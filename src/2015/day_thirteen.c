#include "../common.h"
#include "../utils/minmax.h"

static const int N_PEOPLE = 9;
static const int MAX_NAME_LENGTH = 50;

static int getPersonId(char names[N_PEOPLE][MAX_NAME_LENGTH],
                       const char* name) {
    for (int i = 0; i < N_PEOPLE; ++i) {
        if (names[i][0] == '\0') {
            // we've not seen this city before.
            strcpy(names[i], name);
            return i;
        } else if (strcmp(names[i], name) == 0) {
            return i;
        }
    }
    // Should be exhaustive.
    assert(false);
    return -1;
}

static void sanitizeName(char* name) {
    char* n = name;
    while (*n != '\0') {
        if (!isalpha(*n)) {
            *n = '\0';
        }
        ++n;
    }
}

static void parseInput(FILE* fp, int happiness[N_PEOPLE][N_PEOPLE]) {
    char names[N_PEOPLE][MAX_NAME_LENGTH];
    memset(names, 0, sizeof(names));

    char nameA[50] = {0};
    char nameB[50] = {0};
    char op[50] = {0};
    int hu = 0;
    while (fscanf(fp,
                  "%s would %s %d happiness units by sitting next to %s.\n",
                  nameA,
                  op,
                  &hu,
                  nameB) != EOF) {
        sanitizeName(nameA);
        sanitizeName(nameB);
        int nameIdA = getPersonId(names, nameA);
        int nameIdB = getPersonId(names, nameB);
        happiness[nameIdA][nameIdB] = (strcmp(op, "gain") == 0 ? 1 : -1) * hu;
    }
}

static int permute(int maxPeople,
                   int happiness[N_PEOPLE][N_PEOPLE],
                   int firstPerson,
                   int lastPerson,
                   int total,
                   uint16_t mask,
                   int (*cmp)(int, int),
                   int base) {
    int answer = base;
    if (mask == (1U << maxPeople) - 1) {
        // Need to handle the wrap, where the last person and the first person's
        // happiness are calculated.
        assert(firstPerson != -1);
        assert(lastPerson != -1);
        return total + happiness[lastPerson][firstPerson] +
               happiness[firstPerson][lastPerson];
    }
    for (int i = 0; i < maxPeople; ++i) {
        uint16_t check = 1U << i;
        if (mask & check) {
            // Already seen.
            continue;
        }

        int totalHappiness;
        if (firstPerson == -1) {
            // this is the first person, so don't change the total, but set both
            // the first and last person values to i.
            totalHappiness = permute(
                maxPeople, happiness, i, i, total, mask | check, cmp, base);
        } else {

            totalHappiness = permute(maxPeople,
                                     happiness,
                                     firstPerson,
                                     i,
                                     total + happiness[lastPerson][i] +
                                         happiness[i][lastPerson],
                                     mask | check,
                                     cmp,
                                     base);
        }
        answer = cmp(totalHappiness, answer);
    }
    return answer;
}

FUNCTION_DEFN_FOR_YDS(2015, thirteen, a) {
    int happiness[N_PEOPLE][N_PEOPLE];
    memset(happiness, 0, sizeof(happiness));

    parseInput(fp, happiness);

    int solution = permute(8, happiness, -1, -1, 0, 0, max, INT_MIN);
    printf("%d\n", solution);
}

FUNCTION_DEFN_FOR_YDS(2015, thirteen, b) {
    int happiness[N_PEOPLE][N_PEOPLE];
    memset(happiness, 0, sizeof(happiness));

    parseInput(fp, happiness);

    int solution = permute(9, happiness, -1, -1, 0, 0, max, INT_MIN);

    printf("%d\n", solution);
}
