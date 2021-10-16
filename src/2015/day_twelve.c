#include "../common.h"
#include "../utils/strings.h"

FUNCTION_DEFN_FOR_YDS(2015, twelve, a) {
    int total = 0;
    bool inNumber = false;
    bool isNegative = false;
    int currentNumber = 0;
    char c;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '-') {
            assert(!inNumber);
            inNumber = true;
            isNegative = true;
        } else if (isdigit(c)) {
            int n = c - '0';
            if (inNumber) {
                currentNumber *= 10;
                currentNumber += n;
            } else {
                inNumber = true;
                currentNumber = n;
            }
        } else {
            if (inNumber) {
                total += ((isNegative ? -1 : 1) * currentNumber);
                currentNumber = 0;
                inNumber = false;
                isNegative = false;
            }
        }
    }
    printf("%d\n", total);
}

typedef struct stackEntry {
    int total;
    bool hasRed;
    char type;
} stackEntry;

static void
cleanupNumber(bool* isNegative, int* currentNumber, stackEntry* stackEntry) {
    int total = *currentNumber * (*isNegative ? -1 : 1);
    stackEntry->total += total;
    *currentNumber = 0;
    *isNegative = false;
}

FUNCTION_DEFN_FOR_YDS(2015, twelve, b) {
    const char red[] = {':', '"', 'r', 'e', 'd', '"'};
    int redPos = 0;

    stackEntry stack[10];
    int stackPos = -1;
    bool inNumber = false;
    bool isNegative = false;
    int currentNumber = 0;
    char c;
    while ((c = fgetc(fp)) != EOF) {
        switch (c) {
        case '[':
        case '{':
            assert(stackPos < 9);
            if (inNumber) {
                cleanupNumber(&isNegative, &currentNumber, &stack[stackPos]);
                inNumber = false;
            }
            stackPos += 1;
            stack[stackPos].hasRed = false;
            stack[stackPos].total = 0;
            stack[stackPos].type = c;
            redPos = 0;
            break;
        case ']':
        case '}': {
            assert(stackPos >= 0);
            assert((c == '}' && stack[stackPos].type == '{') ||
                   (c == ']' && stack[stackPos].type == '['));
            if (inNumber) {
                cleanupNumber(&isNegative, &currentNumber, &stack[stackPos]);
                inNumber = false;
            }
            if (stackPos == 0) {
                // we're about to pop off the last item, so its total is the
                // answer.
                printf("%d\n", stack[stackPos].total);
                return;
            } else {
                if (!stack[stackPos].hasRed) {
                    stack[stackPos - 1].total += stack[stackPos].total;
                }
                --stackPos;
            }
            redPos = 0;
            break;
        }
        case '-':
            assert(!inNumber);
            inNumber = true;
            isNegative = true;
            redPos = 0;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            int n = c - '0';
            if (inNumber) {
                currentNumber *= 10;
                currentNumber += n;
            } else {
                inNumber = true;
                currentNumber = n;
            }
            redPos = 0;
            break;
        }
        default:
            if (inNumber) {
                cleanupNumber(&isNegative, &currentNumber, &stack[stackPos]);
                inNumber = false;
            }
            if (c == red[redPos]) {
                if (redPos == 5) {
                    assert(stack[stackPos].type == '{');
                    stack[stackPos].hasRed = true;
                    redPos = 0;
                } else {
                    ++redPos;
                }
            } else {
                redPos = 0;
            }
            break;
        }
    }
}
