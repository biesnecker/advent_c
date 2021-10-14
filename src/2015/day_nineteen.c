#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#include "../macros.h"
#include "../utils/minmax.h"
#include "../utils/rand.h"
#include "../utils/strings.h"
#include "../utils/vector.h"
#include "../utils/xxhash.h"

#define MAX_ALTERNATIVES 10
#define MAX_REPLACEMENTS 15
#define MAX_PAIRS 150
#define MAX_HASHES 1000

typedef struct alternative {
    char* text;
    size_t len;
} alternative;

typedef struct replacement {
    char fst, snd;
    int alts;
    alternative alternatives[MAX_ALTERNATIVES];
} replacement;

static int compareReplacement(const void* a, const void* b) {
    const replacement* x = (const replacement*)a;
    const replacement* y = (const replacement*)b;
    if (x->fst < y->fst) {
        return -1;
    } else if (x->fst > y->fst) {
        return 1;
    } else {
        if (x->snd < y->snd) {
            return -1;
        } else if (x->snd > y->snd) {
            return 1;
        } else {
            return 0;
        }
    }
}

static int compareHashes(const void* a, const void* b) {
    uint64_t x = *(const uint64_t*)a;
    uint64_t y = *(const uint64_t*)b;

    if (x < y) {
        return -1;
    } else if (x > y) {
        return 1;
    } else {
        return 0;
    }
}

// Returns the length of replacements after processing.
static void parseInput(FILE* fp, vector* replacements, char** molecule) {
    bool parsingAlternatives = true;
    char buffer[512];
    char left[50];
    char right[50];
    int ridx = -1;

    replacement* currentReplacement = NULL;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strlen(buffer) == 1 && buffer[0] == '\n') {
            parsingAlternatives = false;
            continue;
        }
        if (parsingAlternatives) {
            assert(sscanf(buffer, "%s => %s", left, right) == 2);
            int keylen = strlen(left);
            char fst, snd;
            if (keylen == 1) {
                fst = left[0];
                snd = 0;
            } else if (keylen == 2) {
                fst = left[0];
                snd = left[1];
            } else {
                assert(false);
            }

            if (currentReplacement == NULL ||
                (currentReplacement->fst != fst ||
                 currentReplacement->snd != snd)) {
                // Setup a new block.
                replacement newReplacement = {
                    .fst = fst, .snd = snd, .alts = 0, .alternatives = {0}};
                vectorPush(replacements, &newReplacement);
                currentReplacement = vectorBack(replacements);
            }
            int aidx = currentReplacement->alts++;
            assert(aidx < MAX_ALTERNATIVES);
            int alength = strlen(right);
            currentReplacement->alternatives[aidx].text = strdup(right);
            assert(currentReplacement->alternatives[aidx].text != NULL);
            currentReplacement->alternatives[aidx].len = alength;
        } else {
            stripNewline(buffer);
            char* moleculeCopy = strdup(buffer);
            assert(moleculeCopy != NULL);
            *molecule = moleculeCopy;
        }
    }
    ++ridx;
    vectorSort(replacements, compareReplacement);
}

FUNCTION_DEFN_FOR_YDS(2015, nineteen, a) {
    vector reps;
    vectorInit(&reps, MAX_REPLACEMENTS, sizeof(replacement));
    char* molecule = NULL;
    parseInput(fp, &reps, &molecule);

    int mlen = strlen(molecule);
    char* start = molecule;
    char* stop = molecule + mlen;

    vector hashes;
    vectorInit(&hashes, MAX_HASHES, sizeof(uint64_t));

    while (molecule < stop) {
        if (isupper(*molecule)) {
            char* matchStart = molecule;
            char fst = *molecule;
            char snd = 0;
            ++molecule;
            if (molecule < stop && islower(*molecule)) {
                snd = *molecule;
                ++molecule;
            }

            replacement key = {
                .fst = fst,
                .snd = snd,
            };
            replacement* r =
                vectorBinarySearch(&reps, &key, compareReplacement);
            if (r != NULL) {
                for (int i = 0; i < r->alts; ++i) {
                    xxhash hsh;
                    xxhashInit(&hsh, 0);
                    // Copy the first part in.
                    xxhashAppend(&hsh, start, matchStart - start);
                    xxhashAppend(
                        &hsh, r->alternatives[i].text, r->alternatives[i].len);
                    xxhashAppend(&hsh, molecule, stop - molecule);
                    uint64_t hash = xxhashDigest(&hsh);
                    vectorPush(&hashes, &hash);
                }
            }
        }
    }

    vectorSort(&hashes, compareHashes);
    uint64_t* hashData = (uint64_t*)vectorDataPtr(&hashes);
    uint64_t lastSeen = 0;
    int solution = 0;
    for (size_t i = 0; i < hashes.len; ++i) {
        if (hashData[i] != lastSeen) {
            ++solution;
            lastSeen = hashData[i];
        }
    }

    printf("%d\n", solution);

    VECTOR_FOR_EACH(&reps, x) {
        replacement* r = x;
        for (int j = 0; j < r->alts; ++j) {
            free(r->alternatives[j].text);
        }
    }
    vectorFinalize(&reps);
    vectorFinalize(&hashes);
}

typedef struct rpair {
    char* left;
    char* right;
    size_t leftLen;
    size_t rightLen;
} rpair;

static void removeSpaces(char* str) {
    int len = strlen(str);
    char* stop = str + len;
    char* writer = strchr(str, ' ');
    if (writer == NULL) {
        // No spaces in the string.
        return;
    }
    char* reader = writer + 1;
    while (reader < stop) {
        char c = *reader++;
        if (c != ' ') {
            *writer++ = c;
        }
    }
    // append the final NUL character at the end.
    *writer = '\0';
}

FUNCTION_DEFN_FOR_YDS(2015, nineteen, b) {
    vector reps;
    vectorInit(&reps, MAX_REPLACEMENTS, sizeof(replacement));
    char* molecule = NULL;
    parseInput(fp, &reps, &molecule);

    vector pairs;
    vectorInit(&pairs, MAX_PAIRS, sizeof(rpair));
    VECTOR_FOR_EACH(&reps, elem) {
        replacement* curr = elem;
        char leftBuf[3] = {curr->fst, 0, 0};
        size_t leftLen = 1;
        if (curr->snd != 0) {
            leftBuf[1] = curr->snd;
            leftLen += 1;
        }
        char* left = strdup(leftBuf);
        for (int j = 0; j < curr->alts; ++j) {
            rpair p = (rpair){
                .left = left,
                .right = curr->alternatives[j].text,
                .leftLen = leftLen,
                .rightLen = curr->alternatives[j].len,
            };
            vectorPush(&pairs, &p);
        }
    }

    rng r;
    rngInit(&r);

    int minReplacements = INT_MAX;
    int nSolutions = 0;

    while (nSolutions < 10) {
        char* m = strdup(molecule);
        vectorShuffle(&pairs, &r);
        int replacementsMade = 0;
        int replacementsMadeOld = 0;
        bool foundE = false;
        do {
            replacementsMadeOld = replacementsMade;
            VECTOR_FOR_EACH(&pairs, p_) {
                rpair* p = p_;

                // Look for the replacement string.
                char* found = strstr(m, p->right);
                if (found != NULL) {
                    ++replacementsMade;
                    for (size_t i = 0; i < p->rightLen; ++i) {
                        // Overwrite the found value with the replacement. The
                        // replacement is always shorter than the found value,
                        // so write spaces in the gap if needed.
                        found[i] = i < p->leftLen ? p->left[i] : ' ';
                        if (p->leftLen == 1 && p->left[0] == 'e') {
                            foundE = true;
                            break;
                        }
                    }
                    removeSpaces(m);
                }
            }
        } while (!foundE && (replacementsMadeOld != replacementsMade));

        if (foundE) {
            ++nSolutions;
            minReplacements = min(minReplacements, replacementsMade);
        }

        free(m);
    }
    printf("Maybe %d?\n", minReplacements);
}
