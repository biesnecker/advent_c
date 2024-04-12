#include "../common.h"

#define NUM_WINNING 10
#define NUM_HAVE 25
#define MAX_CARDS 220

typedef struct _Card {
    int num;
    int winning[NUM_WINNING];
    int have[NUM_HAVE];
} Card;

static void
readInput(FILE* fp, void (*handler)(const char*, void*), void* userData) {
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        handler(buffer, userData);
    }
}

static int intcmp(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

static void readCard(const char* buffer, Card* card) {
    if (sscanf(buffer,
               "Card %d: %d %d %d %d %d %d %d %d %d %d | %d %d %d %d %d %d %d "
               "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
               &card->num,
               &card->winning[0],
               &card->winning[1],
               &card->winning[2],
               &card->winning[3],
               &card->winning[4],
               &card->winning[5],
               &card->winning[6],
               &card->winning[7],
               &card->winning[8],
               &card->winning[9],
               &card->have[0],
               &card->have[1],
               &card->have[2],
               &card->have[3],
               &card->have[4],
               &card->have[5],
               &card->have[6],
               &card->have[7],
               &card->have[8],
               &card->have[9],
               &card->have[10],
               &card->have[11],
               &card->have[12],
               &card->have[13],
               &card->have[14],
               &card->have[15],
               &card->have[16],
               &card->have[17],
               &card->have[18],
               &card->have[19],
               &card->have[20],
               &card->have[21],
               &card->have[22],
               &card->have[23],
               &card->have[24]) != 36) {
        fprintf(stderr, "Failed to parse input\n");
        exit(1);
    }
    qsort(card->winning, NUM_WINNING, sizeof(int), intcmp);
    qsort(card->have, NUM_HAVE, sizeof(int), intcmp);
}

static int nWinners(const Card* c) {
    int count = 0;
    for (int i = 0; i < NUM_WINNING; ++i) {
        if (bsearch(&c->winning[i], c->have, NUM_HAVE, sizeof(int), intcmp) !=
            NULL) {
            ++count;
        }
    }
    return count;
}

static void handlerA(const char* buffer, void* userData) {
    int* total = (int*)userData;
    Card c = {0};
    readCard(buffer, &c);
    int winners = nWinners(&c);
    if (winners > 0) {
        *total += 1 << (winners - 1);
    }
}

typedef struct _Deck {
    Card cards[MAX_CARDS];
    int cardC;
} Deck;

static void handlerB(const char* buffer, void* userData) {
    Deck* deck = (Deck*)userData;
    readCard(buffer, &deck->cards[deck->cardC++]);
}

static int dfs(int cardIdx, const Deck* deck, int* cache) {
    if (cache[cardIdx] > 0) {
        return cache[cardIdx];
    }
    int winners = nWinners(&deck->cards[cardIdx]);
    if (winners == 0) {
        cache[cardIdx] = 1;
        return 1;
    }
    int total = 1;
    for (int i = cardIdx + 1; i <= cardIdx + winners; ++i) {
        total += dfs(i, deck, cache);
    }
    cache[cardIdx] = total;
    return total;
}

FUNCTION_DEFN_FOR_YDS(2023, four, a) {
    int total = 0;
    readInput(fp, handlerA, &total);
    printf("%d\n", total);
}

FUNCTION_DEFN_FOR_YDS(2023, four, b) {
    Deck deck = {0};
    readInput(fp, handlerB, &deck);
    int cache[MAX_CARDS] = {0};
    int total = 0;
    for (int i = 0; i < deck.cardC; ++i) {
        total += dfs(i, &deck, cache);
    }
    printf("%d\n", total);
}
