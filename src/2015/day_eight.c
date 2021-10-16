#include "../common.h"
#include "../utils/strings.h"

FUNCTION_DEFN_FOR_YDS(2015, eight, a) {
    char buffer[100] = {0};

    int totalCodeLen = 0;
    int totalTextLen = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        int codelen = stripNewline(buffer);
        int textlen = 0;
        buffer[codelen - 1] = '\0';

        // Ignore the first and last quotes.
        // int len = codelen - 2;
        const char* s = buffer + 1;
        bool inEscape = false;

        while (*s != '\0') {
            char curr = s[0];
            if (curr == '\\') {
                if (inEscape) {
                    // this is the second of an escaped backspace. Set prev to
                    // 0 so the next time we don't think we're still in an
                    // escape sequence.
                    inEscape = false;
                    ++textlen;
                } else {
                    // Don't increment the textlen until we figure out what the
                    // escape resolves to.
                    inEscape = true;
                }
            } else if (curr == '"') {
                assert(inEscape);
                // This is an escaped quote.
                inEscape = false;
                ++textlen;
            } else if (curr == 'x' && inEscape) {
                // This is an acii escape, like \x00
                inEscape = false;
                assert(isxdigit(s[1]));
                assert(isxdigit(s[2]));
                s += 2;
                ++textlen;
            } else {
                // Regular character
                ++textlen;
            }
            ++s;
        }
        totalCodeLen += codelen;
        totalTextLen += textlen;
    }
    printf("%d\n", totalCodeLen - totalTextLen);
}

FUNCTION_DEFN_FOR_YDS(2015, eight, b) {
    char buffer[100] = {0};

    int totalTextLen = 0;
    int totalEncodedLen = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        int textLen = stripNewline(buffer);
        // Starts with two to account for the outer quotes.
        int encodedLen = 2;
        for (int i = 0; i < textLen; ++i) {
            switch (buffer[i]) {
            case '\\':
                encodedLen += 2;
                break;
            case '"':
                encodedLen += 2;
                break;
            default:
                encodedLen += 1;
                break;
            }
        }
        totalTextLen += textLen;
        totalEncodedLen += encodedLen;
    }
    printf("%d\n", totalEncodedLen - totalTextLen);
}
