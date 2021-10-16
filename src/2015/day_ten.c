#include "../common.h"
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

static node* nodeListFromString(const char* s, int len) {
    int val = s[0] - '0';
    node* listHead = newNode(val, 1);
    node* listTail = listHead;

    for (int i = 1; i < len; ++i) {
        val = s[i] - '0';
        if (listTail->num == val) {
            ++listTail->count;
        } else {
            node* n = newNode(val, 1);
            listTail->next = n;
            listTail = n;
        }
    }
    return listHead;
}

static node* step(node* old) {
    // Create dummy head.
    node newHead = {
        .count = 0,
        .num = 0,
        .next = NULL,
    };
    node* newTail = &newHead;
    while (old != NULL) {
        if (old->count == newTail->num) {
            ++newTail->count;
        } else {
            newTail->next = newNode(old->count, 1);
            newTail = newTail->next;
        }
        if (old->num == newTail->num) {
            ++newTail->count;
        } else {
            newTail->next = newNode(old->num, 1);
            newTail = newTail->next;
        }
        old = old->next;
    }
    return newHead.next;
}

static int getLength(node* head) {
    int total = 0;
    while (head != NULL) {
        total += head->count;
        head = head->next;
    }
    return total;
}

static int runGameAndCleanupNodes(node* head, int rounds) {
    for (int i = 0; i < rounds; ++i) {
        node* newHead = step(head);
        cleanupNodes(head);
        head = newHead;
    }

    int solution = getLength(head);
    cleanupNodes(head);
    return solution;
}

FUNCTION_DEFN_FOR_YDS(2015, ten, a) {
    char buffer[100] = {0};
    fgets(buffer, sizeof(buffer), fp);
    int len = stripNewline(buffer);

    node* list = nodeListFromString(buffer, len);

    int solution = runGameAndCleanupNodes(list, 40);
    printf("%d\n", solution);
}

FUNCTION_DEFN_FOR_YDS(2015, ten, b) {
    char buffer[100] = {0};
    fgets(buffer, sizeof(buffer), fp);
    int len = stripNewline(buffer);

    node* list = nodeListFromString(buffer, len);

    int solution = runGameAndCleanupNodes(list, 50);
    printf("%d\n", solution);
}
