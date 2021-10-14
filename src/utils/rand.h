#pragma once

#include <stdint.h>

typedef struct rng {
    uint64_t state;
    uint64_t inc;
} rng;

void rngInit(rng* r);

uint32_t rngRandom(rng* r);

// Upper bound is inclusive and must be > 0
uint32_t rngRandomBounded(rng* r, uint32_t upperBound);
