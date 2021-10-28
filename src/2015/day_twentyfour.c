#include "../common.h"
#include "../utils/vector.h"

static void readInput(FILE* fp, vector* nums, uint32_t* sum) {
    uint32_t n;
    while (fscanf(fp, "%u\n", &n) != EOF) {
        vectorPush(nums, &n);
        *sum += n;
    }
}

static void combinations(vector* nums,
                         uint32_t r,
                         uint32_t sum,
                         uint32_t groups,
                         uint64_t* minQE) {
    assert(sum % groups == 0);
    uint32_t target = sum / groups;
    uint32_t n = nums->len;
    assert(r > 0);
    assert(r <= n);
    uint32_t* combos = calloc(r, sizeof(uint32_t));
    for (unsigned i = 0; i < r; ++i) {
        combos[i] = i;
    }
    uint32_t i = r - 1;
    while (combos[0] < n - r + 1) {
        while (i > 0 && combos[i] == n - r + i) {
            --i;
        }

        // Do shit with the combo.
        uint32_t total = 0;
        uint64_t qe = 1;
        for (unsigned j = 0; j < r; ++j) {
            uint32_t num = *(uint32_t*)vectorAt(nums, combos[j]);
            total += num;
            qe *= num;
        }
        if (total == target && qe < *minQE) {
            *minQE = qe;
        }

        combos[i]++;
        while (i < r - 1) {
            combos[i + i] = combos[i] + 1;
            i++;
        }
    }
}

FUNCTION_DEFN_FOR_YDS(2015, twentyfour, a) {
    uint32_t sum = 0;
    vector nums;
    vectorInit(&nums, 30, sizeof(uint32_t), NULL);
    readInput(fp, &nums, &sum);
    assert(nums.len <= 64);

    uint64_t minQE = UINT64_MAX;
    uint32_t groupSize = 1;
    while (true) {
        combinations(&nums, groupSize++, sum, 3, &minQE);
        if (minQE != UINT64_MAX) {
            printf("%llu\n", minQE);
            break;
        }
    }

    vectorFinalize(&nums);
}

FUNCTION_DEFN_FOR_YDS(2015, twentyfour, b) {
    uint32_t sum = 0;
    vector nums;
    vectorInit(&nums, 30, sizeof(uint32_t), NULL);
    readInput(fp, &nums, &sum);
    assert(nums.len <= 64);

    uint64_t minQE = UINT64_MAX;
    uint32_t groupSize = 1;
    while (true) {
        combinations(&nums, groupSize++, sum, 4, &minQE);
        if (minQE != UINT64_MAX) {
            printf("%llu\n", minQE);
            break;
        }
    }

    vectorFinalize(&nums);
}
