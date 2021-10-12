#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "../macros.h"

static int getAttributeId(const char* attribute) {
    switch (attribute[0]) {
    case 'c':
        if (attribute[2] == 'i') {
            return 0; // children
        } else if (attribute[2] == 't') {
            return 1; // cats
        } else if (attribute[2] == 'r') {
            return 8; // cars
        }
        break;
    case 's':
        return 2; // samoyeds
        break;
    case 'p':
        if (attribute[1] == 'o') {
            return 3; // pomeranians
        } else if (attribute[1] == 'e') {
            return 9; // perfumes
        }
        break;
    case 'a':
        return 4; // akitas
        break;
    case 'v':
        return 5; // vizslas
        break;
    case 'g':
        return 6; // golfdish
        break;
    case 't':
        return 7; // trees
        break;
    }
    assert(false);
}

static const int ticker[] = {
    3, // children
    7, // cats
    2, // samoyeds
    3, // pomeranians
    0, // akitas
    0, // vizslas
    5, // goldfish
    3, // trees
    2, // cars
    1, // perfumes
};

static bool isMatch(const char* attribute, int value) {
    int attrId = getAttributeId(attribute);
    return ticker[attrId] == value;
}

static bool isMatchRange(const char* attribute, int value) {
    int attrId = getAttributeId(attribute);
    switch (attrId) {
    case 1: // cats
    case 7: // trees
        return ticker[attrId] < value;
    case 3: // pomeranians
    case 6: // goldfish
        return ticker[attrId] > value;
    default:
        return ticker[attrId] == value;
    }
}

FUNCTION_DEFN_FOR_YDS(2015, sixteen, a) {
    int sueId = 0;
    char attrOne[50] = {0};
    char attrTwo[50] = {0};
    char attrThree[50] = {0};
    int attrOneValue = 0;
    int attrTwoValue = 0;
    int attrThreeValue = 0;

    // Sue 10: perfumes: 5, pomeranians: 4, children: 9

    while (fscanf(fp,
                  "Sue %d: %[a-z]: %d, %[a-z]: %d, %[a-z]: %d\n",
                  &sueId,
                  attrOne,
                  &attrOneValue,
                  attrTwo,
                  &attrTwoValue,
                  attrThree,
                  &attrThreeValue) != EOF) {
        if (isMatch(attrOne, attrOneValue) && isMatch(attrTwo, attrTwoValue) &&
            isMatch(attrThree, attrThreeValue)) {
            // If all pass then this is the match.
            printf("%d\n", sueId);
            break;
        }
    }
}

FUNCTION_DEFN_FOR_YDS(2015, sixteen, b) {
    int sueId = 0;
    char attrOne[50] = {0};
    char attrTwo[50] = {0};
    char attrThree[50] = {0};
    int attrOneValue = 0;
    int attrTwoValue = 0;
    int attrThreeValue = 0;

    // Sue 10: perfumes: 5, pomeranians: 4, children: 9

    while (fscanf(fp,
                  "Sue %d: %[a-z]: %d, %[a-z]: %d, %[a-z]: %d\n",
                  &sueId,
                  attrOne,
                  &attrOneValue,
                  attrTwo,
                  &attrTwoValue,
                  attrThree,
                  &attrThreeValue) != EOF) {
        if (isMatchRange(attrOne, attrOneValue) &&
            isMatchRange(attrTwo, attrTwoValue) &&
            isMatchRange(attrThree, attrThreeValue)) {
            // If all pass then this is the match.
            printf("%d\n", sueId);
            break;
        }
    }
}
