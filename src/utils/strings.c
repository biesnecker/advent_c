#include "strings.h"

#include <string.h>

size_t stripNewline(char* s) {
    size_t len = strlen(s);
    if (s[len - 1] == '\n') {
        s[len - 1] = '\0';
        len -= 1;
    }
    return len;
}

bool isPrefix(const char* prefix, const char* str) {
    return strncmp(prefix, str, strlen(prefix)) == 0;
}
