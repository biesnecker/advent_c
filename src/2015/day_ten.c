#include <assert.h>
#include <stdlib.h>

#include "../macros.h"
#include "../utils/strings.h"

typedef struct node {
    struct node* next;
    int num;
    int count;
} node;

static node* newNode(int num, int count) {
    node* n = malloc(sizeof(node));
    assert(n != NULL);
    n->num = num;
    n->count = count;
    n->next = NULL;
    return n;
}

static void cleanupNodes(node* head) {
    node* next = head;
    while (next != NULL) {
        node* current = next;
        next = current->next;
        free(current);
    }
}

static void debugPrintList(node* head) {
    node* next = head;
    while (next != NULL) {
        printf("{%d : %d}->", next->count, next->num);
        next = next->next;
    }
    puts("");
}

FUNCTION_DEFN_FOR_YDS(2015, ten, a) {
    char buffer[100] = {0};
    fgets(buffer, sizeof(buffer), fp);
    int len = stripNewline(buffer);
    printf("%s\n", buffer);

    int val = buffer[0] - '0';
    node* oldListHead = newNode(val, 1);
    node* oldListTail = oldListHead;

    for (int i = 1; i < len; ++i) {
        val = buffer[i] - '0';
        if (oldListTail->num == val) {
            ++oldListTail->count;
        } else {
            node* n = newNode(val, 1);
            oldListTail->next = n;
            oldListTail = n;
        }
    }

    debugPrintList(oldListHead);
    cleanupNodes(oldListHead);
}

FUNCTION_DEFN_FOR_YDS(2015, ten, b) {
    UNUSED_ARG(fp);
}
