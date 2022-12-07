#include "../common.h"

#define MAX_CHILDREN 16

typedef struct _dir_node {
    struct _dir_node* children[MAX_CHILDREN];
    struct _dir_node* parent;
    char* name;
    int filesize;
    int child_count;
    int list_count;
    int total_size;
} dir_node;

typedef struct {
    dir_node* root;
    dir_node* cwd;
} state;

static dir_node* newDirectory(dir_node* parent, const char* name) {
    dir_node* d = calloc(1, sizeof(dir_node));
    assert(d != NULL);
    d->name = strdup(name);
    assert(d->name != NULL);
    d->parent = parent;
    return d;
}

static void freeDirectory(dir_node* d) {
    free(d->name);
    for (int i = 0; i < d->child_count; i++) {
        freeDirectory(d->children[i]);
        free(d->children[i]);
    }
}

static void readInput(FILE* fp, state* s) {
    char buffer[64];

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strcmp(buffer, "$ ls\n") == 0) {
            assert(s->cwd != NULL);
            s->cwd->list_count += 1;
            continue;
        }
        char strBuffer[64];
        int size;
        if (sscanf(buffer, "$ cd %s", strBuffer) == 1) {
            // Changing directories.
            if (strBuffer[0] == '/') {
                // This command is only used once, at the top of the input,
                // so we'll use it to signal the creation of the root.
                assert(s->root == NULL);
                s->root = newDirectory(NULL, strBuffer);
                s->cwd = s->root;
            } else if (strBuffer[0] == '.' && strBuffer[1] == '.') {
                // Go back up a node.
                assert(s->cwd != NULL && s->cwd->parent != NULL);
                s->cwd = s->cwd->parent;
            } else {
                // Find the child directory and enter into that.
                bool found = false;
                for (int i = 0; i < s->cwd->child_count; ++i) {
                    if (strcmp(strBuffer, s->cwd->children[i]->name) == 0) {
                        s->cwd = s->cwd->children[i];
                        found = true;
                        break;
                    }
                }
                assert(found);
            }
        } else if (sscanf(buffer, "dir %s", strBuffer) == 1 &&
                   s->cwd->list_count < 2) {
            // Creating a directory.
            assert(s->cwd != NULL && s->cwd->child_count < MAX_CHILDREN);
            s->cwd->children[s->cwd->child_count++] =
                newDirectory(s->cwd, strBuffer);
        } else if (sscanf(buffer, "%d %s", &size, strBuffer) == 2 &&
                   s->cwd->list_count < 2) {
            // Creating a file.
            assert(s->cwd != NULL);
            s->cwd->filesize += size;
        }
    }
}

static int calcTotalSize(dir_node* d) {
    if (d == NULL) {
        return 0;
    }
    int dsize = d->filesize;
    for (int i = 0; i < d->child_count; i++) {
        dsize += calcTotalSize(d->children[i]);
    }
    d->total_size = dsize;
    return dsize;
}

static void calcSumOfDirs(const dir_node* d, int maxSize, int* sum) {
    if (d == NULL) {
        return;
    }
    if (d->total_size <= maxSize) {
        *sum += d->total_size;
    }
    for (int i = 0; i < d->child_count; i++) {
        calcSumOfDirs(d->children[i], maxSize, sum);
    }
}

static void
findSmallestToDelete(const dir_node* d, int minSize, int* smallestFound) {
    if (d == NULL) {
        return;
    }
    if (d->total_size < minSize) {
        // If it's already too small no need to recurse into its children.
        return;
    }
    if (d->total_size < *smallestFound) {
        *smallestFound = d->total_size;
    }
    for (int i = 0; i < d->child_count; i++) {
        findSmallestToDelete(d->children[i], minSize, smallestFound);
    }
}

FUNCTION_DEFN_FOR_YDS(2022, seven, a) {
    state s = {0};
    readInput(fp, &s);
    calcTotalSize(s.root);

    int sumOfDirs = 0;
    calcSumOfDirs(s.root, 100000, &sumOfDirs);
    printf("%d\n", sumOfDirs);
    freeDirectory(s.root);
}

FUNCTION_DEFN_FOR_YDS(2022, seven, b) {
    state s = {0};
    readInput(fp, &s);
    calcTotalSize(s.root);

    const int totalDiskSpace = 70000000;
    const int neededDiskSpace = 30000000;
    const int mustFreeSpace =
        neededDiskSpace - (totalDiskSpace - s.root->total_size);

    int smallestFound = INT_MAX;
    findSmallestToDelete(s.root, mustFreeSpace, &smallestFound);
    printf("%d\n", smallestFound);
}
