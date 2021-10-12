#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"

typedef struct _solution {
    const char* puzzle_id;
    void (*solution)(FILE*);
    const char* input_filename;
} solution;

#define SOLUTION_FOR_YDS(year_name, day_name, side)                            \
    {                                                                          \
        .puzzle_id = STRINGIFY(YDS(year_name, day_name, side)),                \
        .solution = &FUNCTION_NAME_FOR_YDS(year_name, day_name, side),         \
        "input/" STRINGIFY(YD(year_name, day_name)) "_input.txt",              \
    }

FUNCTION_DECL_FOR_YDS(2015, one, a);
FUNCTION_DECL_FOR_YDS(2015, one, b);
FUNCTION_DEFN_FOR_YDS(2015, two, a);
FUNCTION_DEFN_FOR_YDS(2015, two, b);
FUNCTION_DEFN_FOR_YDS(2015, three, a);
FUNCTION_DEFN_FOR_YDS(2015, three, b);
FUNCTION_DEFN_FOR_YDS(2015, four, a);
FUNCTION_DEFN_FOR_YDS(2015, four, b);
FUNCTION_DEFN_FOR_YDS(2015, five, a);
FUNCTION_DEFN_FOR_YDS(2015, five, b);
FUNCTION_DEFN_FOR_YDS(2015, six, a);
FUNCTION_DEFN_FOR_YDS(2015, six, b);
FUNCTION_DEFN_FOR_YDS(2015, seven, a);
FUNCTION_DEFN_FOR_YDS(2015, seven, b);
FUNCTION_DEFN_FOR_YDS(2015, eight, a);
FUNCTION_DEFN_FOR_YDS(2015, eight, b);
FUNCTION_DEFN_FOR_YDS(2015, nine, a);
FUNCTION_DEFN_FOR_YDS(2015, nine, b);
FUNCTION_DEFN_FOR_YDS(2015, ten, a);
FUNCTION_DEFN_FOR_YDS(2015, ten, b);
FUNCTION_DEFN_FOR_YDS(2015, eleven, a);
FUNCTION_DEFN_FOR_YDS(2015, eleven, b);
FUNCTION_DEFN_FOR_YDS(2015, twelve, a);
FUNCTION_DEFN_FOR_YDS(2015, twelve, b);
FUNCTION_DEFN_FOR_YDS(2015, thirteen, a);
FUNCTION_DEFN_FOR_YDS(2015, thirteen, b);
FUNCTION_DEFN_FOR_YDS(2015, fourteen, a);
FUNCTION_DEFN_FOR_YDS(2015, fourteen, b);
FUNCTION_DEFN_FOR_YDS(2015, fifteen, a);
FUNCTION_DEFN_FOR_YDS(2015, fifteen, b);
FUNCTION_DEFN_FOR_YDS(2015, sixteen, a);
FUNCTION_DEFN_FOR_YDS(2015, sixteen, b);
FUNCTION_DEFN_FOR_YDS(2015, seventeen, a);
FUNCTION_DEFN_FOR_YDS(2015, seventeen, b);
FUNCTION_DEFN_FOR_YDS(2015, eighteen, a);
FUNCTION_DEFN_FOR_YDS(2015, eighteen, b);

static solution solutions[] = {
    SOLUTION_FOR_YDS(2015, one, a),       SOLUTION_FOR_YDS(2015, one, b),
    SOLUTION_FOR_YDS(2015, two, a),       SOLUTION_FOR_YDS(2015, two, b),
    SOLUTION_FOR_YDS(2015, three, a),     SOLUTION_FOR_YDS(2015, three, b),
    SOLUTION_FOR_YDS(2015, four, a),      SOLUTION_FOR_YDS(2015, four, b),
    SOLUTION_FOR_YDS(2015, five, a),      SOLUTION_FOR_YDS(2015, five, b),
    SOLUTION_FOR_YDS(2015, six, a),       SOLUTION_FOR_YDS(2015, six, b),
    SOLUTION_FOR_YDS(2015, seven, a),     SOLUTION_FOR_YDS(2015, seven, b),
    SOLUTION_FOR_YDS(2015, eight, a),     SOLUTION_FOR_YDS(2015, eight, b),
    SOLUTION_FOR_YDS(2015, nine, a),      SOLUTION_FOR_YDS(2015, nine, b),
    SOLUTION_FOR_YDS(2015, ten, a),       SOLUTION_FOR_YDS(2015, ten, b),
    SOLUTION_FOR_YDS(2015, eleven, a),    SOLUTION_FOR_YDS(2015, eleven, b),
    SOLUTION_FOR_YDS(2015, twelve, a),    SOLUTION_FOR_YDS(2015, twelve, b),
    SOLUTION_FOR_YDS(2015, thirteen, a),  SOLUTION_FOR_YDS(2015, thirteen, b),
    SOLUTION_FOR_YDS(2015, fourteen, a),  SOLUTION_FOR_YDS(2015, fourteen, b),
    SOLUTION_FOR_YDS(2015, fifteen, a),   SOLUTION_FOR_YDS(2015, fifteen, b),
    SOLUTION_FOR_YDS(2015, sixteen, a),   SOLUTION_FOR_YDS(2015, sixteen, b),
    SOLUTION_FOR_YDS(2015, seventeen, a), SOLUTION_FOR_YDS(2015, seventeen, b),
    SOLUTION_FOR_YDS(2015, eighteen, a),  SOLUTION_FOR_YDS(2015, eighteen, b)};

static int n_solutions = sizeof(solutions) / sizeof(solutions[0]);

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
