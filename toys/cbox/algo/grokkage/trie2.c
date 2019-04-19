#include "types.h"

trie_t *new() {
    int i;
    trie_t *trie = malloc(sizeof(trie_t));
    for (i = 0; i < TRIE_SIZE; i++) trie->m[i] = NULL;
    return trie;
}

void insert(trie_t *root, char *key) {
    printf("insert: %s\n", key);
    int len = strlen(key);
    trie_t *t = root;
    int i;
    for (i = 0; i < len; i++) {
        int idx = key[i] - 'a';
        if (t->m[idx] == NULL) { t->m[idx] = new(); }
        t = t->m[idx]; // advance to next trie
    }
    int idx = TERMINATOR; // terminate all strings here
    t->m[idx] = new();
}

int find(trie_t *root, char *key) {
    printf("find: %s => ", key);
    int len = strlen(key);
    trie_t *t = root;
    int i;
    for (i = 0; i < len; i++) {
        int idx = key[i] - 'a';
        //printf("check %c => %d\n", key[i], idx);
        if (t->m[idx] == NULL) {
            printf("not found\n");
            return 0;
        }
        t = t->m[idx];
    }
    int idx = TERMINATOR; //'\0' - 'a';
    if (t->m[idx] == NULL) { // was key terminated?
        printf("not found\n");
        return 0;
    } else {
        printf("found\n");
        return 1;
    }
}

int main()
{
    trie_t *root = new();
    insert(root, "bart");       insert(root, "cindy");
    insert(root, "grant");      insert(root, "chad");
    insert(root, "mackenzie");  insert(root, "clarissa");

    find(root, "chad"); find(root, "mackenzie");
    find(root, "cind"); find(root, "bar");
    find(root, "cha");  find(root, "mackenzi");
}
