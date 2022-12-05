#include "../common.h"

#define STACK_MAX 64
#define N_STACKS 9

typedef enum {
    ADD_TO_STACK,
    REVERSE_STACKS,
    MOVE_CRATES,
} cmd_type;

typedef struct {
    int stack_id;
    char crate;
} add_to_stack_cmd;

typedef struct {
    int count;
    int from;
    int to;
} move_crates_cmd;

typedef struct {
    cmd_type type;
    union {
        add_to_stack_cmd add;
        move_crates_cmd move;
    };
} cmd;

typedef struct {
    char crates[STACK_MAX];
    int tos;
} stack;

typedef struct {
    stack stacks[N_STACKS];
} boat;

typedef struct {
    boat b;
    int move_type;
} state;

static char stack_peek(const stack* s) {
    assert(s->tos > 0);
    return s->crates[s->tos - 1];
}

static char stack_pop(stack* s) {
    assert(s->tos > 0);
    return s->crates[(s->tos--) - 1];
}

static void stack_push(stack* s, char c) {
    assert(s->tos < STACK_MAX);
    s->crates[s->tos++] = c;
}

static void stack_reverse(stack* s) {
    if (s->tos < 2) {
        return;
    }
    int head = 0;
    int tail = s->tos - 1;
    while (head < tail) {
        char tmp = s->crates[head];
        s->crates[head] = s->crates[tail];
        s->crates[tail] = tmp;
        head += 1;
        tail -= 1;
    }
}

static void
readInput(FILE* fp, void (*handler)(const cmd*, void*), void* userData) {
    char buffer[500];
    int state = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (state == 0) {
            // We're parsing the initial layout of the crates.
            bool foundAny = false;
            char* tokpos;
            int offset = 0;
            while ((tokpos = strchr(buffer + offset, '[')) != NULL) {
                foundAny = true;
                int tokenIdx = (int)(tokpos - (char*)buffer);
                assert(tokenIdx % 4 == 0);
                int stack_id = tokenIdx / 4;

                cmd c = {.type = ADD_TO_STACK,
                         .add.crate = tokpos[1],
                         .add.stack_id = stack_id};
                handler(&c, userData);
                offset = tokenIdx + 3;
            }
            if (!foundAny) {
                // We're in the line below the stacks where the stack numbers
                // are. We'll skip this line and reverse in the next blank line.
                state += 1;
            }
        } else if (state == 1) {
            // We're in the blank line between the initial layout and the move
            // commands. We'll use this opportunity to reverse the stacks that
            // we've built up so that we're ready to take move commands.
            cmd c = {.type = REVERSE_STACKS};
            handler(&c, userData);
            state += 1;
        } else {
            // Parse the move commands.
            cmd c = {.type = MOVE_CRATES};
            int count, to, from;
            assert(
                sscanf(buffer, "move %d from %d to %d", &count, &from, &to) ==
                3);
            c.move.count = count;
            // Decrement from and to so that the stacks are 0-indexed.
            c.move.from = --from;
            c.move.to = --to;
            handler(&c, userData);
        }
    }
}

static void handler(const cmd* c, void* userData) {
    state* s = (state*)userData;
    boat* b = &s->b;

    switch (c->type) {
    case ADD_TO_STACK: {
        stack_push(&b->stacks[c->add.stack_id], c->add.crate);
        break;
    }
    case REVERSE_STACKS: {
        for (int i = 0; i < N_STACKS; ++i) {
            stack_reverse(&b->stacks[i]);
        }
        break;
    }
    case MOVE_CRATES: {
        if (s->move_type == 0) {
            for (int i = 0; i < c->move.count; ++i) {
                char crate = stack_pop(&b->stacks[c->move.from]);
                stack_push(&b->stacks[c->move.to], crate);
            }
        } else {
            char buffer[64] = {0};
            int buffer_idx = 0;
            for (int i = 0; i < c->move.count; ++i) {
                buffer[buffer_idx++] = stack_pop(&b->stacks[c->move.from]);
            }
            while (buffer_idx > 0) {
                stack_push(&b->stacks[c->move.to], buffer[--buffer_idx]);
            }
        }
        break;
    }
    }
}

FUNCTION_DEFN_FOR_YDS(2022, five, a) {
    state s = {.b = {0}, .move_type = 0};
    readInput(fp, handler, &s);
    char res[N_STACKS + 1] = {0};
    for (int i = 0; i < N_STACKS; ++i) {
        res[i] = stack_peek(&s.b.stacks[i]);
    }
    printf("%s\n", res);
}

FUNCTION_DEFN_FOR_YDS(2022, five, b) {
    state s = {.b = {0}, .move_type = 1};
    readInput(fp, handler, &s);
    char res[N_STACKS + 1] = {0};
    for (int i = 0; i < N_STACKS; ++i) {
        res[i] = stack_peek(&s.b.stacks[i]);
    }
    printf("%s\n", res);
}
