#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "../macros.h"
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

FUNCTION_DEFN_FOR_YDS(2015, twelve, b) {
    UNUSED_ARG(fp);
}
