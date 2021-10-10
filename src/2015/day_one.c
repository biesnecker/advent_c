#include <stdbool.h>

#include "../macros.h"

FUNCTION_DEFN_FOR_YDS(2015, one, a) {
    char paren;
    int sum = 0;
    while (true) {
        paren = (char)fgetc(fp);
        if (paren == EOF) {
            break;
        }
        switch (paren) {
        case '(':
            ++sum;
            break;
        case ')':
            --sum;
            break;
        }
    }
    printf("%d\n", sum);
}

FUNCTION_DEFN_FOR_YDS(2015, one, b) {
    char paren;
    int pos = 1;
    int sum = 0;
    while (true) {
        paren = (char)fgetc(fp);
        if (paren == EOF) {
            break;
        }
        switch (paren) {
        case '(':
            ++sum;
            break;
        case ')':
            --sum;
            break;
        }
        if (sum == -1) {
            printf("%d\n", pos);
            break;
        }
        ++pos;
    }
}
