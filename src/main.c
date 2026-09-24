#include "stdio.h"
#include "radix_uint32_trie.h"

int main() {
    struct Node *root = radix_uint32_trie_init();
    radix_uint32_trie_add(root, (struct BinaryValue) {
        .bits = 100,
        .length = 3
    });

    radix_uint32_trie_add(root, (struct BinaryValue) {
        .bits = 001,
        .length = 3
    });

    radix_uint32_trie_add(root, (struct BinaryValue) {
        .bits = 011,
        .length = 3
    });

    radix_uint32_trie_add(root, (struct BinaryValue) {
        .bits = 000,
        .length = 3
    });

    radix_uint32_trie_add(root, (struct BinaryValue) {
        .bits = 1101,
        .length = 3
    });
    return 0;
}