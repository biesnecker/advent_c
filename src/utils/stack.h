#pragma once
#include <assert.h>
#include <stdbool.h>

// Macros for creating and manipulating stacks.

#define __STACK_STRUCT(name, elem_type, capacity)                              \
    static const int _stack_##name##_capacity = capacity;                      \
    typedef struct _##name {                                                   \
        elem_type elems[capacity];                                             \
        int tos;                                                               \
    } name

#define __STACK_EMPTY(name)                                                    \
    static bool name##_empty(const name* s) { return s->tos == 0; }

#define __STACK_SIZE(name)                                                     \
    static int name##_size(const name* s) { return s->tos; }

#define __STACK_POP(name, elem_type)                                           \
    static elem_type name##_pop(name* s) {                                     \
        assert(s->tos > 0);                                                    \
        return s->elems[(s->tos--) - 1];                                       \
    }

#define __STACK_PUSH(name, elem_type)                                          \
    static void name##_push(name* s, elem_type v) {                            \
        assert(s->tos < _stack_##name##_capacity);                             \
        s->elems[s->tos++] = v;                                                \
    }

#define __STACK_PEEK(name, elem_type)                                          \
    static elem_type name##_peek(const name* s) {                              \
        assert(s->tos > 0);                                                    \
        return s->elems[s->tos - 1];                                           \
    }

#define __STACK_PEEK_REF(name, elem_type)                                      \
    static elem_type* name##_peek_ref(name* s) {                               \
        assert(s->tos > 0);                                                    \
        return &s->elems[s->tos - 1];                                          \
    }

#define __STACK_REVERSE(name, elem_type)                                       \
    static void name##_reverse(name* s) {                                      \
        if (s->tos < 2)                                                        \
            return;                                                            \
        int head = 0;                                                          \
        int tail = s->tos - 1;                                                 \
        while (head < tail) {                                                  \
            elem_type tmp = s->elems[head];                                    \
            s->elems[head] = s->elems[tail];                                   \
            s->elems[tail] = tmp;                                              \
            head += 1;                                                         \
            tail -= 1;                                                         \
        }                                                                      \
    }

#define STACK_DEFINE(name, elem_type, capacity)                                \
    __STACK_STRUCT(name, elem_type, capacity);                                 \
    __STACK_EMPTY(name);                                                       \
    __STACK_SIZE(name);                                                        \
    __STACK_POP(name, elem_type);                                              \
    __STACK_PUSH(name, elem_type);                                             \
    __STACK_PEEK(name, elem_type);                                             \
    __STACK_PEEK_REF(name, elem_type);                                         \
    __STACK_REVERSE(name, elem_type)
