#include "../common.h"
#include "../utils/strings.h"

static char* readInput(FILE* fp) {
    char buffer[50];
    assert(fgets(buffer, sizeof(buffer), fp) != NULL);
    stripNewline(buffer);
    return strdup(buffer);
}

static char* checksum(const char* code, size_t len) {
    char* checksum = strdup(code);
    while (len % 2 == 0) {
        len >>= 1;
        char* newChecksum = calloc(len + 1, sizeof(char));
        for (size_t i = 0; i < len; ++i) {
            if (checksum[i * 2] == checksum[i * 2 + 1]) {
                newChecksum[i] = '1';
            } else {
                newChecksum[i] = '0';
            }
        }
        free(checksum);
        checksum = newChecksum;
    }
    return checksum;
}

static size_t expand(char** inP, size_t len) {
    size_t newLen = (len * 2) + 1;
    char* firstHalf = *inP;
    char* output = calloc(newLen + 1, sizeof(char));
    memcpy(output, firstHalf, len);
    output[len] = '0';
    for (size_t i = 0; i < len; ++i) {
        char c = firstHalf[i] == '0' ? '1' : '0';
        output[newLen - (1 + i)] = c;
    }
    free(firstHalf);
    *inP = output;
    return newLen;
}

static void findSolution(FILE* fp, size_t goal) {
    char* code = readInput(fp);
    size_t codeSize = strlen(code);
    while (codeSize < goal) {
        codeSize = expand(&code, codeSize);
    }
    char* cs = checksum(code, goal);
    printf("%s\n", cs);
    free(cs);
    free(code);
}

FUNCTION_DEFN_FOR_YDS(2016, sixteen, a) {
    findSolution(fp, 272);
}

FUNCTION_DEFN_FOR_YDS(2016, sixteen, b) {
    findSolution(fp, 35651584);
}
