#include <assert.h>
#include <string.h>

#include "../macros.h"
#include "../utils/vector.h"

typedef enum opcode {
    INC,
    JMP,
    JIO,
    JIE,
    TPL,
    HLF,
} opcode;

typedef enum reg { NONE, A, B } reg;

typedef struct instr {
    opcode op;
    reg r;
    int arg;
} instr;

static void parseNoArg(const char* s, instr* i) {
    char buf[5];
    char c;
    assert(sscanf(s, "%s %c\n", buf, &c) == 2);
    if (strcmp(buf, "inc") == 0) {
        i->op = INC;
    } else if (strcmp(buf, "tpl") == 0) {
        i->op = TPL;
    } else if (strcmp(buf, "hlf") == 0) {
        i->op = HLF;
    } else {
        assert(false);
    }
    i->arg = 0;
    switch (c) {
    case 'a':
        i->r = A;
        break;
    case 'b':
        i->r = B;
        break;
    default:
        assert(false);
    }
}

static void parseArg(const char* s, instr* i) {
    char buf[5];
    char c;
    int arg;
    assert(sscanf(s, "%s %c, %d\n", buf, &c, &arg) == 3);
    if (strcmp(buf, "jie") == 0) {
        i->op = JIE;
    } else if (strcmp(buf, "jio") == 0) {
        i->op = JIO;
    } else {
        assert(false);
    }
    i->arg = arg;
    switch (c) {
    case 'a':
        i->r = A;
        break;
    case 'b':
        i->r = B;
        break;
    default:
        assert(false);
    }
}

static void parseJmp(const char* s, instr* i) {
    int arg;
    assert(sscanf(s, "jmp %d\n", &arg) == 1);
    i->op = JMP;
    i->r = NONE;
    i->arg = arg;
}

static void readInput(FILE* fp, vector* prog) {
    char buffer[50];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        instr i = {0};
        switch (buffer[0]) {
        case 'i': // INC
        case 't': // TPL
        case 'h': // HLF
            parseNoArg(buffer, &i);
            break;
        case 'j':
            if (buffer[1] == 'm') {
                parseJmp(buffer, &i);
            } else {
                parseArg(buffer, &i);
            }
            break;
        default:
            assert(false);
        }
        vectorPush(prog, &i);
    }
}

static void runInterpreter(vector* prog, uint32_t* a, uint32_t* b) {
    int progLen = (int)prog->len;
    int pc = 0;
    while (true) {
        if (pc >= progLen || pc < 0) {
            return;
        }
        instr* i = vectorAt(prog, pc);
        uint32_t* r;
        switch (i->r) {
        case A:
            r = a;
            break;
        case B:
            r = b;
            break;
        default:
            r = NULL;
        }

        switch (i->op) {
        case INC:
            *r += 1;
            ++pc;
            break;
        case TPL:
            *r *= 3;
            ++pc;
            break;
        case HLF:
            *r /= 2;
            ++pc;
            break;
        case JMP:
            pc += i->arg;
            break;
        case JIO:
            if (*r == 1) {
                pc += i->arg;
            } else {
                ++pc;
            }
            break;
        case JIE:
            if (*r % 2 == 0) {
                pc += i->arg;
            } else {
                ++pc;
            }
            break;
        }
    }
}

FUNCTION_DEFN_FOR_YDS(2015, twentythree, a) {
    vector prog;
    vectorInit(&prog, 20, sizeof(instr));

    readInput(fp, &prog);

    uint32_t registerA = 0;
    uint32_t registerB = 0;

    runInterpreter(&prog, &registerA, &registerB);

    printf("%u\n", registerB);

    vectorFinalize(&prog);
}

FUNCTION_DEFN_FOR_YDS(2015, twentythree, b) {
    vector prog;
    vectorInit(&prog, 20, sizeof(instr));

    readInput(fp, &prog);

    uint32_t registerA = 1;
    uint32_t registerB = 0;

    runInterpreter(&prog, &registerA, &registerB);

    printf("%u\n", registerB);

    vectorFinalize(&prog);
}
