#include <ctype.h>

#include "../common.h"
#include "../utils/vector.h"

typedef struct emu {
    int registers[4];
} emu;

typedef enum op {
    COPY_L2R,
    COPY_R2R,
    INC,
    DEC,
    JNZ,
    JMP,
} op;

typedef struct instruction {
    op opcode;
    int arg0;
    int arg1;
} instruction;

static bool _isRegister(char r) {
    return r == 'a' || r == 'b' || r == 'c' || r == 'd';
}

static inline int _registerIndex(char r) {
    return r - 'a';
}

static void readInput(FILE* fp,
                      void (*handler)(const instruction*, void* userData),
                      void* userData) {
    char buffer[64];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        instruction i = {0};
        char* spointer = strchr(buffer, ' ');
        assert(spointer != NULL);
        spointer += 1;
        switch (buffer[0]) {
        case 'c': {
            if (isdigit(*spointer)) {
                i.opcode = COPY_L2R;
                int lit;
                char r;
                assert(sscanf(spointer, "%d %c", &lit, &r) == 2);
                assert(_isRegister(r));
                i.arg0 = lit;
                i.arg1 = _registerIndex(r);
            } else {
                i.opcode = COPY_R2R;
                char r1, r2;
                assert(sscanf(spointer, "%c %c", &r1, &r2) == 2);
                assert(_isRegister(r1));
                assert(_isRegister(r1));
                i.arg0 = _registerIndex(r1);
                i.arg1 = _registerIndex(r2);
            }
            break;
        }
        case 'j': {
            if (isdigit(*spointer)) {
                int lit1, lit2;
                assert(sscanf(spointer, "%d %d", &lit1, &lit2) == 2);
                if (lit1 == 0) {
                    continue;
                } else {
                    i.opcode = JMP;
                    i.arg0 = lit2;
                }
            } else {
                char r;
                int lit;
                assert(sscanf(spointer, "%c %d", &r, &lit) == 2);
                assert(_isRegister(r));
                i.opcode = JNZ;
                i.arg0 = _registerIndex(r);
                i.arg1 = lit;
            }
            break;
        }
        case 'i':
            i.opcode = INC;
            assert(_isRegister(*spointer));
            i.arg0 = _registerIndex(*spointer);
            break;
        case 'd':
            i.opcode = DEC;
            assert(_isRegister(*spointer));
            i.arg0 = _registerIndex(*spointer);
            break;
        default:
            assert(false);
        }

        handler(&i, userData);
    }
}

static void simulate(const vector* program, emu* e) {
    size_t pc = 0;
    while (true) {
        const instruction* i = VECTOR_AT(program, pc);
        switch (i->opcode) {
        case COPY_L2R:
            e->registers[i->arg1] = i->arg0;
            ++pc;
            break;
        case COPY_R2R:
            e->registers[i->arg1] = e->registers[i->arg0];
            ++pc;
            break;
        case INC:
            e->registers[i->arg0] += 1;
            ++pc;
            break;
        case DEC:
            e->registers[i->arg0] -= 1;
            ++pc;
            break;
        case JNZ:
            if (e->registers[i->arg0] != 0) {
                pc += i->arg1;
            } else {
                ++pc;
            }
            break;
        case JMP:
            pc += i->arg0;
            break;
        default:
            assert(false);
        }
        if (pc >= VECTOR_SIZE(program)) {
            break;
        }
    }
}

static void inputHandler(const instruction* i, void* userData) {
    vector* iv = userData;
    vectorPush(iv, i);
}

static void run(FILE* fp, emu* e) {
    vector instructions;
    vectorInit(&instructions, 32, sizeof(instruction));

    readInput(fp, inputHandler, &instructions);
    simulate(&instructions, e);
    printf("%d\n", e->registers[0]);

    vectorFinalize(&instructions);
}

FUNCTION_DEFN_FOR_YDS(2016, twelve, a) {
    emu e = {0};
    run(fp, &e);
}

FUNCTION_DEFN_FOR_YDS(2016, twelve, b) {
    emu e = {.registers = {0, 0, 1, 0}};
    run(fp, &e);
}
