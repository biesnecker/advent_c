#include "../common.h"
#include "../utils/strings.h"

#define BUFFER_SIZE 8192

static size_t readInput(FILE* fp, char* buffer) {
    assert(fgets(buffer, BUFFER_SIZE, fp) != NULL);
    return stripNewline(buffer);
}

static size_t decompress(const char* s, size_t len, bool recursive) {
    size_t segmentLength = 0;

    const char* stop = s + len;
    while (s < stop) {
        if (*s != '(') {
            ++segmentLength;
            ++s;
            continue;
        }
        ++s; // skip over the open paren
        unsigned run, rep;
        assert(sscanf(s, "%ux%u", &run, &rep) == 2);

        s = memchr(s, ')', stop - s);
        assert(s != NULL);
        ++s;
        // s is now at the beginning of the repeated segment.
        if (recursive) {
            segmentLength += rep * decompress(s, run, recursive);
        } else {
            segmentLength += run * rep;
        }

        // Finally skip over the repeated segment.
        // Skip over the repeated segment.
        assert(s + run <= stop);
        s += run;
    }
    return segmentLength;
}

FUNCTION_DEFN_FOR_YDS(2016, nine, a) {
    char buffer[8192];
    size_t inputLen = readInput(fp, buffer);

    printf("%zu\n", decompress(buffer, inputLen, false));
}

FUNCTION_DEFN_FOR_YDS(2016, nine, b) {
    char buffer[8192];
    size_t inputLen = readInput(fp, buffer);

    printf("%zu\n", decompress(buffer, inputLen, true));
}
