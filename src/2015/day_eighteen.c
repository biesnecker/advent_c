#include "../common.h"
#include "../utils/strings.h"

static const int BOARD_SIZE = 100;
static const int TURNS = 100;

static void parseInput(FILE* fp, uint8_t board[BOARD_SIZE][BOARD_SIZE]) {
    int line = 0;
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        assert(line < BOARD_SIZE);
        stripNewline(buffer);
        char* p = buffer;
        int col = 0;
        while (*p != '\0') {
            assert(col < BOARD_SIZE);
            board[line][col] = *p == '.' ? 0 : 1;
            ++col;
            ++p;
        }
        ++line;
    }
}

static bool
isOnNextStep(uint8_t board[BOARD_SIZE][BOARD_SIZE], int x, int y, bool stuck) {
    if (stuck && ((x == 0 && y == 0) || (x == 0 && y == BOARD_SIZE - 1) ||
                  (x == BOARD_SIZE - 1 && y == 0) ||
                  (x == BOARD_SIZE - 1 && y == BOARD_SIZE - 1))) {
        return true;
    }
    bool isOn = board[x][y] == 1;

    int count = 0;
    for (int a = x - 1; a < x + 2; ++a) {
        if (a < 0 || a > BOARD_SIZE - 1) {
            continue;
        }
        for (int b = y - 1; b < y + 2; ++b) {
            if (b < 0 || b > BOARD_SIZE - 1) {
                continue;
            }
            if (a == x && b == y) {
                continue;
            }

            count += board[a][b];
        }
    }
    if (isOn && (count == 2 || count == 3)) {
        return true;
    } else if (!isOn && count == 3) {
        return true;
    }
    return false;
}

FUNCTION_DEFN_FOR_YDS(2015, eighteen, a) {
    uint8_t board[BOARD_SIZE][BOARD_SIZE];
    memset(board, 0, sizeof(board));
    parseInput(fp, board);
    for (int turn = 0; turn < TURNS; ++turn) {
        uint8_t newBoard[BOARD_SIZE][BOARD_SIZE];
        memset(newBoard, 0, sizeof(newBoard));
        for (int x = 0; x < BOARD_SIZE; ++x) {
            for (int y = 0; y < BOARD_SIZE; ++y) {
                if (isOnNextStep(board, x, y, false /* stuck */)) {
                    newBoard[x][y] = 1;
                }
            }
        }
        assert(sizeof(board) == sizeof(newBoard));
        memcpy(board, newBoard, sizeof(board));
    }

    int solution = 0;
    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            solution += board[x][y];
        }
    }
    printf("%d\n", solution);
}

FUNCTION_DEFN_FOR_YDS(2015, eighteen, b) {
    uint8_t board[BOARD_SIZE][BOARD_SIZE];
    memset(board, 0, sizeof(board));
    parseInput(fp, board);

    // Turn on the stuck lights.
    board[0][0] = 1;
    board[0][BOARD_SIZE - 1] = 1;
    board[BOARD_SIZE - 1][0] = 1;
    board[BOARD_SIZE - 1][BOARD_SIZE - 1] = 1;

    for (int turn = 0; turn < TURNS; ++turn) {
        uint8_t newBoard[BOARD_SIZE][BOARD_SIZE];
        memset(newBoard, 0, sizeof(newBoard));
        for (int x = 0; x < BOARD_SIZE; ++x) {
            for (int y = 0; y < BOARD_SIZE; ++y) {
                if (isOnNextStep(board, x, y, true /* stuck */)) {
                    newBoard[x][y] = 1;
                }
            }
        }
        assert(sizeof(board) == sizeof(newBoard));
        memcpy(board, newBoard, sizeof(board));
    }

    int solution = 0;
    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            solution += board[x][y];
        }
    }
    printf("%d\n", solution);
}
