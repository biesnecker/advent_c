#pragma once

#include <assert.h>
#include <stdbool.h>

// Macros for creating and manipulating queues. Implemented as a
// ring buffer.

#define __QUEUE_CAPACITY_NAME(name) _queue_##name##_capacity

#define __QUEUE_CAPACITY(name, capacity)                                       \
    static const int __QUEUE_CAPACITY_NAME(name) = capacity

#define __QUEUE_STRUCT(name, elem_type, capacity)                              \
    __QUEUE_CAPACITY(name, capacity);                                          \
    typedef struct _##name {                                                   \
        elem_type elems[capacity];                                             \
        int writeIdx, readIdx;                                                 \
        bool isFull;                                                           \
    } name

#define __QUEUE_EMPTY(name)                                                    \
    static bool name##_empty(const name* q) {                                  \
        return q->writeIdx == q->readIdx && !q->isFull;                        \
    }

#define __QUEUE_POP(name, elem_type)                                           \
    static elem_type name##_pop(name* q) {                                     \
        assert(!name##_empty(q));                                              \
        q->isFull = false;                                                     \
        elem_type e = q->elems[q->readIdx];                                    \
        q->readIdx = (q->readIdx + 1) % __QUEUE_CAPACITY_NAME(name);           \
        return e;                                                              \
    }

#define __QUEUE_PUSH(name, elem_type)                                          \
    static void name##_push(name* q, elem_type e) {                            \
        assert(!q->isFull);                                                    \
        q->elems[q->writeIdx] = e;                                             \
        q->writeIdx = (q->writeIdx + 1) % __QUEUE_CAPACITY_NAME(name);         \
        q->isFull = q->writeIdx == q->readIdx;                                 \
    }

#define QUEUE_DEFINE(name, elem_type, capacity)                                \
    __QUEUE_STRUCT(name, elem_type, capacity);                                 \
    __QUEUE_EMPTY(name);                                                       \
    __QUEUE_POP(name, elem_type);                                              \
    __QUEUE_PUSH(name, elem_type);
