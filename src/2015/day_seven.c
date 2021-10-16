#include "../common.h"
#include "../utils/strings.h"

#define NODECNT 858

typedef struct node {
    bool resolved;
    union {
        uint16_t value;
        char* source;
    };
} node;

static uint16_t identifierCode(const char* id) {
    int len = strlen(id);
    assert(len > 0 && len < 3);
    uint16_t a, b;
    a = (uint16_t)(id[0] - 96);
    if (strlen(id) == 2) {
        b = (uint16_t)(id[1] - 96);
    } else {
        b = 0;
    }
    return a + (b << 5) - 1;
}

static uint16_t resolve(node* nodes, const char* id) {
    uint16_t code = identifierCode(id);

    node* n = &nodes[code];

    // Already resolved, just return the value.
    if (n->resolved) {
        return n->value;
    }

    uint16_t val;
    char bufferOne[10] = {0};
    char bufferTwo[10] = {0};
    char bufferThree[10] = {0};

    // Handle cases where there is a single value, either a constant or an
    // identifier.
    if (strchr(n->source, ' ') == NULL) {
        // The simplest case is that it's a constant number.
        if (sscanf(n->source, "%hu", &val) == 1) {
            free(n->source);
            n->source = NULL;
            n->resolved = true;
            n->value = val;
            return n->value;
        }

        // There's another simple case where one identifier feeds into the next.
        if (sscanf(n->source, "%s", bufferOne) == 1) {
            val = resolve(nodes, bufferOne);
            if (!n->resolved) {
                free(n->source);
                n->source = NULL;
                n->resolved = true;
                n->value = val;
            }
            return val;
        }

        // Should be one or the other.
        assert(false);
    }

    // Handle NOT, which is always followed by an identifier.
    if (n->source[0] == 'N') {
        assert(sscanf(n->source, "NOT %s", bufferOne) == 1);
        val = resolve(nodes, bufferOne);
        // Need to make sure that this node wasn't already resolved.
        if (!n->resolved) {
            free(n->source);
            n->source = NULL;
            n->resolved = true;

            // Binary NOT
            n->value = ~val;
        }
        return n->value;
    }

    // All other values are in the form op1 OP op2, where either op1 or op2
    // can be literals. We're going to parse those into parts.
    const char* firstSpace = strchr(n->source, ' ');
    const char* secondSpace = strrchr(n->source, ' ');
    assert(firstSpace != secondSpace);

    size_t sourceLen = strlen(n->source);
    ptrdiff_t opOneLen = firstSpace - n->source;
    ptrdiff_t opTwoLen = (n->source + sourceLen) - (secondSpace + 1);
    ptrdiff_t opLen = secondSpace - (firstSpace + 1);
    memcpy(bufferOne, n->source, opOneLen);
    memcpy(bufferTwo, firstSpace + 1, opLen);
    memcpy(bufferThree, secondSpace + 1, opTwoLen);

    uint16_t lhs, rhs;
    // Resolve the left and right sides.
    if (sscanf(bufferOne, "%hu", &lhs) != 1) {
        lhs = resolve(nodes, bufferOne);
    }
    if (sscanf(bufferThree, "%hu", &rhs) != 1) {
        rhs = resolve(nodes, bufferThree);
    }
    if (!n->resolved) {
        free(n->source);
        n->source = NULL;
        n->resolved = true;
        if (strcmp(bufferTwo, "AND") == 0) {
            n->value = lhs & rhs;
        } else if (strcmp(bufferTwo, "OR") == 0) {
            n->value = lhs | rhs;
        } else if (strcmp(bufferTwo, "LSHIFT") == 0) {
            n->value = lhs << rhs;
        } else if (strcmp(bufferTwo, "RSHIFT") == 0) {
            n->value = lhs >> rhs;
        } else {
            // Exhaustive.
            assert(false);
        }
    }

    return n->value;
}

static void parseInput(FILE* fp, node* nodes) {
    char buffer[100] = {0};
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        stripNewline(buffer);
        char* arrowLoc = strrchr(buffer, '>');
        uint16_t code = identifierCode(arrowLoc + 2);
        ptrdiff_t sourceLen = arrowLoc - buffer - 2;

        nodes[code].source = calloc(sourceLen, 1);
        memcpy(nodes[code].source, buffer, sourceLen);
    }
}

static void cleanupNodes(node* nodes, int nodeCnt) {
    // Cleanup unresolved nodes.
    for (int i = 0; i < nodeCnt; ++i) {
        if (!nodes[i].resolved) {
            free(nodes[i].source);
            nodes[i].source = NULL;
        }
    }
}

FUNCTION_DEFN_FOR_YDS(2015, seven, a) {
    node nodes[NODECNT] = {0};
    parseInput(fp, nodes);

    uint16_t solution = resolve(nodes, "a");
    printf("%u\n", solution);

    cleanupNodes(nodes, NODECNT);
}

FUNCTION_DEFN_FOR_YDS(2015, seven, b) {
    node nodes[NODECNT] = {0};
    parseInput(fp, nodes);

    // Now we need to override the value of b.
    uint16_t bCode = identifierCode("b");
    free(nodes[bCode].source);
    nodes[bCode].resolved = true;
    nodes[bCode].value = 3176;

    uint16_t solution = resolve(nodes, "a");
    printf("%u\n", solution);

    cleanupNodes(nodes, NODECNT);
}
