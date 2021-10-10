#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../macros.h"
#include "../utils/md5.h"

FUNCTION_DEFN_FOR_YDS(2015, four, a) {
    char key[20] = {0};
    char* kp = key;
    while (true) {
        char c = fgetc(fp);
        if (c == '\n' || c == EOF) {
            break;
        }
        *kp = c;
        kp++;
    }
    int counter = 0;
    uint32_t hash[] = {0, 0, 0, 0};
    while (true) {
        char buffer[100] = {0};
        int len = sprintf(buffer, "%s%d", key, counter);
        md5((uint8_t*)buffer, len, (uint8_t*)hash);
        if ((hash[0] & 0xf0ffff) == 0) {
            printf("%d\n", counter);
            break;
        }
        ++counter;
    }
}

FUNCTION_DEFN_FOR_YDS(2015, four, b) {
    char key[20] = {0};
    char* kp = key;
    while (true) {
        char c = fgetc(fp);
        if (c == '\n' || c == EOF) {
            break;
        }
        *kp = c;
        kp++;
    }
    int counter = 0;
    uint32_t hash[] = {0, 0, 0, 0};
    while (true) {
        char buffer[100] = {0};
        int len = sprintf(buffer, "%s%d", key, counter);
        md5((uint8_t*)buffer, len, (uint8_t*)hash);
        if ((hash[0] & 0xffffff) == 0) {
            printf("%d\n", counter);
            break;
        }
        ++counter;
    }
}
