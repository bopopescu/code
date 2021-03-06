#include "types.hpp"

using namespace std;

shared_ptr<trie_t> new_trie() {
    shared_ptr<trie_t> trie = make_shared<trie_t>();
    for (int i = 0; i < TRIE_SIZE; i++) trie->m[i] = nullptr;
    return trie;
}

void insert(shared_ptr<trie_t> &root, string &key) {
    printf("insert: %s\n", key.c_str());
    int len = key.size();
    auto ptr = root.get();
    for (int i = 0; i < len; i++) {
        int idx = key[i] - 'a';
        if (ptr->m[idx] == nullptr) { ptr->m[idx] = new_trie(); }
        ptr = ptr->m[idx].get(); // advance to next trie
    }
    int idx = TERMINATOR; // terminate all strings here
    ptr->m[idx] = new_trie();
}

bool find(const shared_ptr<trie_t> &root, string &key) {
    printf("find: %s => ", key.c_str());
    int len = key.size();
    auto ptr = root.get();
    for (int i = 0; i < len; i++) {
        int idx = key[i] - 'a';
        if (ptr->m[idx] == nullptr) {
            printf("not found\n");
            return false;
        }
        ptr = ptr->m[idx].get();
    }
    int idx = TERMINATOR;
    if (ptr->m[idx] == nullptr) { // was key terminated?
        printf("not found\n");
        return false;
    } else {
        printf("found\n");
        return true;
    }
}

int main()
{
    shared_ptr<trie_t> root = new_trie();
    string key;
    key = "bart"; insert(root, key);
    key = "cindy"; insert(root, key);
    key = "douche"; find(root, key);
    key = "bar"; find(root, key);
    key = "cindy"; find(root, key);
}

