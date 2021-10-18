#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"

typedef struct _solution {
    const char* puzzle_id;
    void (*solution)(FILE*);
    const char* input_filename;
} solution;

FUNCTION_DECL_FOR_YEAR(2015);
FUNCTION_DECL_FOR_YD(2016, one);
FUNCTION_DECL_FOR_YD(2016, two);
FUNCTION_DECL_FOR_YD(2016, three);
FUNCTION_DECL_FOR_YD(2016, four);
FUNCTION_DECL_FOR_YD(2016, five);
FUNCTION_DECL_FOR_YD(2016, six);
FUNCTION_DECL_FOR_YD(2016, seven);
FUNCTION_DECL_FOR_YD(2016, eight);
FUNCTION_DECL_FOR_YD(2016, nine);

static solution solutions[] = {SOLUTIONS_FOR_YEAR(2015),
                               SOLUTIONS_FOR_YD(2016, one),
                               SOLUTIONS_FOR_YD(2016, two),
                               SOLUTIONS_FOR_YD(2016, three),
                               SOLUTIONS_FOR_YD(2016, four),
                               SOLUTIONS_FOR_YD(2016, five),
                               SOLUTIONS_FOR_YD(2016, six),
                               SOLUTIONS_FOR_YD(2016, seven),
                               SOLUTIONS_FOR_YD(2016, eight),
                               SOLUTIONS_FOR_YD(2016, nine)};

static int n_solutions = ARRAY_LEN(solutions);

void printUsage(const char* programName) {
    fprintf(stderr,
            "Advent of Code solutions in pure C\n\n"
            "USAGE: %s PUZZLE_ID\n\n"
            "Available puzzles:\n",
            programName);
    for (int i = 0; i < n_solutions; ++i) {
        fprintf(stderr, "\t%s\n", solutions[i].puzzle_id);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }
    const char* puzzleId = argv[1];
    for (int i = 0; i < n_solutions; ++i) {
        if (strcmp(solutions[i].puzzle_id, puzzleId) == 0) {
            const char* filename = solutions[i].input_filename;
            FILE* fp = fopen(filename, "r");
            if (fp == NULL) {
                fprintf(stderr, "Unable to open file: %s\n", filename);
                return EXIT_FAILURE;
            }
            solutions[i].solution(fp);
            fclose(fp);
            return EXIT_SUCCESS;
        }
    }

    fprintf(stderr, "Invalid puzzle_id: %s\n", puzzleId);
    printUsage(argv[0]);
    return EXIT_FAILURE;
}
