#include "rand.h"

#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

void rngInit(rng* r) {
    int fd = open("/dev/urandom", O_RDONLY);
    assert(fd > 0);
    assert(read(fd, r, sizeof(rng)) > 0);
    assert(close(fd) == 0);
}

uint32_t rngRandom(rng* r) {
    uint64_t oldstate = r->state;
    r->state = oldstate * 6364136223846793005ULL + (r->inc | 1);
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

uint32_t rngRandomBounded(rng* r, uint32_t upperBound) {
    assert(upperBound > 0);
    const uint64_t maxRandom =
        ((uint64_t)UINT32_MAX + 1) -
        (((uint64_t)UINT32_MAX + 1) % (uint64_t)upperBound);

    uint32_t repCnt = 0;
    uint32_t res;
    do {
        res = rngRandom(r);
        ++repCnt;
    } while (res >= maxRandom && repCnt < 10);

    return res % upperBound;
}
