#include "../common.h"

typedef enum {
    ROCK,
    PAPER,
    SCISSORS,
} move;

// Returns 1 if you won, -1 if your opponent won, and 0 it was a tie.
static int checkWinner(move opp, move you) {
    if (opp == you) {
        return 0;
    } else if (opp == ROCK) {
        return you == PAPER ? 1 : -1;
    } else if (opp == PAPER) {
        return you == SCISSORS ? 1 : -1;
    } else {
        return you == ROCK ? 1 : -1;
    }
}

static int scoreForMove(move m) {
    switch (m) {
    case ROCK:
        return 1;
    case PAPER:
        return 2;
    case SCISSORS:
        return 3;
    }
}

static move moveFromChar(char c) {
    switch (c) {
    case 'A':
    case 'X':
        return ROCK;
    case 'B':
    case 'Y':
        return PAPER;
    case 'C':
    case 'Z':
        return SCISSORS;
    default:
        assert(false);
    }
}

static move expectedMove(move opp, char c) {
    switch (c) {
    case 'X':
        // You need to lose
        return opp == ROCK ? SCISSORS : opp == PAPER ? ROCK : PAPER;
    case 'Y':
        // You need to draw
        return opp;
    case 'Z':
        // You need to win
        return opp == ROCK ? PAPER : opp == PAPER ? SCISSORS : ROCK;
    default:
        assert(false);
    }
}

static int scoreRound(move opp, move you) {
    int score = scoreForMove(you);
    switch (checkWinner(opp, you)) {
    case 1:
        // you won
        return score + 6;
    case 0:
        // you tied
        return score + 3;
    case -1:
        // you lost
        return score;
    default:
        assert(false);
    }
}

static void
readInput(FILE* fp, void (*handler)(char, char, void*), void* userData) {
    char buffer[5];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        handler(buffer[0], buffer[2], userData);
    }
}

static void handlerA(char a, char b, void* userData) {
    const move opp = moveFromChar(a);
    const move you = moveFromChar(b);
    int* score = (int*)userData;
    *score += scoreRound(opp, you);
}

static void handlerB(char a, char b, void* userData) {
    const move opp = moveFromChar(a);
    const move you = expectedMove(opp, b);
    int* score = (int*)userData;
    *score += scoreRound(opp, you);
}

FUNCTION_DEFN_FOR_YDS(2022, two, a) {
    int totalScore = 0;
    readInput(fp, handlerA, &totalScore);
    printf("%d\n", totalScore);
}

FUNCTION_DEFN_FOR_YDS(2022, two, b) {
    int totalScore = 0;
    readInput(fp, handlerB, &totalScore);
    printf("%d\n", totalScore);
}
